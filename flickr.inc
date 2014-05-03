<?php
/**
 * @file
 * The Flickr API functions.
 */

define('FLICKR_REST_ENDPOINT', 'https://api.flickr.com/services/rest/');
require_once(drupal_get_path('module', 'flickr') . '/flickr.api.inc');

/**
 * A list of possible photo sizes with description and label.
 *
 * @return array
 *   An array of photo sizes.
 */
function flickr_photo_sizes() {
  return array(
    's' => array(
      'label' => 'Square',
      'description' => t('s: 75 px square'),
    ),
    't' => array(
      'label' => 'Thumbnail',
      'description' => t('t: 100 px on longest side'),
    ),
    'q' => array(
      'label' => 'Large Square',
      'description' => t('q: 150 px square'),
    ),
    'm' => array(
      'label' => 'Small',
      'description' => t('m: 240 px on longest side'),
    ),
    'n' => array(
      'label' => 'Small 320',
      'description' => t('n: 320 px on longest side'),
    ),
    '-' => array(
      'label' => 'Medium',
      'description' => t('-: 500 px on longest side'),
    ),
    'z' => array(
      'label' => 'Medium 640',
      'description' => t('z: 640 px on longest side'),
    ),
    'c' => array(
      'label' => 'Medium 800',
      'description' => t('c: 800 px on longest side'),
    ),
    'b' => array(
      'label' => 'Large',
      'description' => t('b: 1024 px on longest side'),
    ),
    'h' => array(
      'label' => 'Large 1600',
      'description' => t('h: 1600 px on longest side'),
    ),
    'k' => array(
      'label' => 'Large 2048',
      'description' => t('k: 2048 px on longest side'),
    ),
    'o' => array(
      'label' => 'Original',
      'description' => t('o: Original image'),
    ),
    'x' => array(
      'label' => 'Flash slideshow',
      'description' => t('x: Full featured responsive slideshow (for group, set and user IDs only)'),
    ),
    'y' => array(
      'label' => 'Simple slideshow',
      'description' => t('y: Basic responsive slideshow (for set and user IDs only)'),
    ),
  );
};

/**
 * Submit a request to Flickr.
 *
 * @param string $method
 *   String method name.
 * @param string $args
 *   Associative array of arguments names and values.
 * @param string $cacheable
 *   Boolean indicating if it's safe cache the results of this request.
 * @param string $return_errors
 *   Boolean indicating if the caller will handle displaying error messages.
 *
 * @return array
 *   an array with the the result of the request, or FALSE on error.
 */
function flickr_request($method, $args, $cacheable = TRUE, $return_errors = FALSE) {
  // Add in additional parameters then sort them for signing.
  $args['api_key'] = trim(variable_get('flickr_api_key', ''));
  $args['method'] = $method;
  $args['format'] = 'php_serial';
  ksort($args);

  // Build an argument hash API signing (we'll also use it for the cache id).
  $arg_hash = '';
  foreach ($args as $k => $v) {
    $arg_hash .= $k . $v;
  }
  // If we've got a secret, sign the arguments.
  if ($secret = trim(variable_get('flickr_api_secret', ''))) {
    $args['api_sig'] = md5($secret . $arg_hash);
  }

  // Build the URL.
  foreach ($args as $k => $v) {
    $encoded_params[] = urlencode($k) . '=' . urlencode($v);
  }
  $url = FLICKR_REST_ENDPOINT . '?' . implode('&', $encoded_params);

  // If it's a cachable request, try to load a cached value.
  if ($cacheable) {
    if ($cache = cache_get("flickr_$arg_hash", 'cache')) {
      // Check that the value is still "fresh".
      if ($cache->expire > time()) {
        return unserialize($cache->data);
      }
    }
  }

  // If a cached value wasn't suitable, attempt to connect and fetch a result.
  $result = drupal_http_request($url);
  if ($result->code != 200) {
    if ($return_errors) {
      return array(
        'stat' => 'error',
        // In Drupal <= 5.1, only HTTP errors are stored in $result->code
        // correctly, not TCP/IP errors. We can not count on this variable being
        // correct until this module requires Drupal 5.2 or above.
        'code' => $result->code,
        'message' => $result->error,
      );
    }
    flickr_set_error(t("Could not connect to Flickr, Error: @error", array('@error' => $result->error)));
    return FALSE;
  }

  // Make sure it unserializes.
  $response = unserialize($result->data);
  if (!$response) {
    if ($return_errors) {
      return array(
        'stat' => 'error',
        'code' => '-1',
        'message' => 'The response was corrupted, it could not be unserialized.',
      );
    }
    flickr_set_error(t("Flickr's response was corrupted and could not be unserialized."));
    return FALSE;
  }

  // Check that the request was successful.
  if (flickr_response_has_error($response)) {
    if ($return_errors) {
      return $response;
    }
    flickr_set_error($response);
    return FALSE;
  }

  // Save cacheable results for future use.
  if ($cacheable) {
    cache_set("flickr_$arg_hash", $result->data, 'cache', time() + variable_get('flickr_cache_duration', 3600));
  }

  return $response;
}

/**
 * This function will try to create an html image tag referencing the Flickr
 * photo with the desired size if that size is available for this photo.
 *
 * @param string $photo
 *   The photo variable.
 * @param string $size
 *   The desired image size.
 * @param string $attributes
 *   An optional array of HTML attributes to pass to the image.
 *
 * @return array
 *   a html image tag referencing the image of the desired
 *   size if it is available.
 */
function flickr_img($photo, $size = NULL, $attributes = NULL) {
  $sizes = flickr_photo_sizes();
  if (!isset($size)) {
    $size = '-';
  }
  if (!isset($sizes[$size])) {
    return;
  }
  if (!isset($attributes) || !is_array($attributes)) {
    $attributes = array();
  }
  if (empty($attributes['class'])) {
    $attributes['class'] = NULL;
  }

  // photoset's use primary instead of id to specify the image.
  if (isset($photo['primary'])) {
    $id = $photo['primary'];
    $attributes['class'] = implode(' ', array(
      $attributes['class'],
      'flickr-photoset-img',
    ));
  }
  else {
    $id = $photo['id'];
    $attributes['class'] = implode(' ', array(
      $attributes['class'],
      'flickr-photo-img',
    ));
  }

  if ($size == 's') {
    $attributes['height'] = $attributes['width'] = 75;
    $img_url = flickr_photo_img($photo, $size);
  }
  else {
    $image_sizes = flickr_photo_get_sizes($id);
    if ($image_sizes) {
      foreach ($image_sizes as $image_size) {
        if ($image_size['label'] == $sizes[$size]['label']) {
          break;
        }
      }
      if (isset($image_size)) {
        $img_url = $image_size['source'];
        $attributes['height'] = $image_size['height'];
        $attributes['width'] = $image_size['width'];
      }
    }
    else {
      $img_url = flickr_photo_img($photo, $size);
    }
  }
  $info = !is_array($photo['title']) ? flickr_photo_get_info($photo['id']) : '';
  $title = is_array($photo['title']) ? str_replace('"', "'", htmlspecialchars_decode(strip_tags($photo['description']['_content']))) : str_replace('"', "'", htmlspecialchars_decode(strip_tags($info['description']['_content'])));
  if (empty($title) == 'title') {
    $title = $photo['title'];
  }
  if (module_exists('jcaption') && (variable_get('flickr_class') != NULL || variable_get('flickr_rel') != NULL)) {
    $title = '';
  }
  return theme('image', array(
    'path' => $img_url,
    'alt' => $title,
    'title' => $title,
    'attributes' => $attributes,
    'getsize' => FALSE,
  ));
}

/**
 * Returns the URL to $photo with size $size using the correct image farm
 * from the $photo variable.
 *
 * @param string $photo
 *   Photo to which the url should point.
 * @param string $size
 *   Size of the photo.
 * @param string $format
 *   Format of the photo.
 *
 * @return array
 *   URL for $photo with the correct size and format.
 */
function flickr_photo_img($photo, $size = NULL, $format = NULL) {
  // Early images don't have a farm setting so default to 1.
  $farm = isset($photo['farm']) ? $photo['farm'] : 1;
  $server = $photo['server'];
  // photoset's use primary instead of id to specify the image.
  $id = isset($photo['primary']) ? $photo['primary'] : $photo['id'];
  $secret = $photo['secret'];

  return "https://farm{$farm}.static.flickr.com/{$server}/{$id}_{$secret}" . ($size ? "_$size." : '.') . ($size == 'o' ? $format : 'jpg');
}

/**
 * Returns the URL for the Flick photo page.
 *
 * @param string $owner
 *   Owner of the photo.
 * @param string $id
 *   ID of the photo to reference in the URL.
 *
 * @return array
 *   URL for the flickr photo page showing photo with $id of $owner.
 */
function flickr_photo_page_url($owner, $id = NULL) {
  $nsid = is_array($owner) ? $owner['nsid'] : $owner;
  if ($person = flickr_people_get_info($nsid)) {
    return $person['photosurl']['_content'] . $id;
  }
  else {
    return "https://flickr.com/photos/$nsid/$id";
  }
}

/**
 * Returns the URL of a given photoset page.
 *
 * @param string $owner
 *   Owner of the photoset.
 * @param string $id
 *   ID of the photoset to which the url must lead.
 *
 * @return array
 *   URL for the photoset page of photoset $id of owner $owner.
 */
function flickr_photoset_page_url($owner, $id = NULL) {
  $nsid = is_array($owner) ? $owner['nsid'] : $owner;
  if ($person = flickr_people_get_info($nsid)) {
    return $person['photosurl']['_content'] . 'sets/' . $id;
  }
  else {
    return "https://flickr.com/photos/$nsid/sets/$id";
  }
}

/**
 * Tries to match an 'identifier' onto a flickr nsid.
 *
 * This function will first see whether $identifier is already
 * a nsid (format check only, no api call).  If it is not and the
 * identifier has the format of an email, an api call will be made to
 * check whether there is an nsid for that email.  If this is not the
 * case, the $identifier is treated as a username and an api call is
 * made to find the nsid for that username.
 *
 * If none of these succeed, the result will be false
 *
 * @param string $identifier
 *   Identifier to find an NSID for.
 *
 * @return array
 *   Valid NSID or false if none can be found.
 */
function flickr_user_find_by_identifier($identifier) {
  if (flickr_is_nsid($identifier)) {
    // Identifier is an NSID.
    return $identifier;
  }
  if (valid_email_address($identifier) && ($user = flickr_user_find_by_email($identifier))) {
    return $user['nsid'];
  }
  if ($user = flickr_user_find_by_username($identifier)) {
    return $user['nsid'];
  }

  return FALSE;
}

/**
 * Flickr is NSID.
 */
function flickr_is_nsid($id) {
  return preg_match('/^\d+@N\d+$/', $id);
}

/**
 * Flickr tag request.
 */
function flickr_tag_request_args($tags = array(), $mode = 'all') {
  $args = array();
  if (!empty($tags)) {
    $args['tags'] = implode(',', $tags);
    $args['tag_mode'] = $mode == 'all' ? $mode : 'any';
  }
  return $args;
}

/**
 * Check if the response from the Flickr api call was an error.
 *
 * @param string $response
 *   Response to check.
 *
 * @return array
 *   True if the response is an error message.
 */
function flickr_response_has_error($response) {
  return !(isset($response['stat']) && $response['stat'] == 'ok');
}

/**
 * Display an error message to flickr admins and write an error to watchdog.
 *
 * @param string $message_or_response
 *   Message or error response to display.
 */
function flickr_set_error($message_or_response) {
  if (is_array($message_or_response)) {
    $message = t('Flickr error @error_id: %flickr_error', array(
      '@error_id' => $message_or_response['code'],
      '%flickr_error' => $message_or_response['message'],
    ));
  }
  else {
    $message = $message_or_response;
  }

  if (user_access('administer flickr')) {
    drupal_set_message($message, 'error');
  }
  watchdog('flickr', $message, array(), WATCHDOG_WARNING);
}

/**
 * Returns Flickr detialed data based on nsid.
 * An elaboration of flickr_people_get_info.
 *
 * @param string $nsid
 *   A valid Flickr numerical user ID. Should be validated before arriving here.
 *
 * @return array $people
 *   HTML with a Flickr user's name that links to their Flickr profile.
 *   Real name if it exists, if not the username.
 */
function flickr_get_user_info($nsid) {
  $info = flickr_people_get_info($nsid);
  $people = array();
  $people['name'] = !empty($info['realname']['_content']) ? l($info['realname']['_content'], $info['profileurl']['_content'], array(
    'attributes' => array(
      'title' => t('View user on Flickr.'),
      'target' => '_blank',
    ),
  )) : l($info['username']['_content'], $info['profileurl']['_content'], array(
    'attributes' => array(
      'title' => t('View user on Flickr.'),
      'target' => '_blank',
    ),
  ));
  $username = strip_tags($people['name']);
  $people['photostream'] = l(t("!user on Flickr", array('!user' => $username)), $info['photosurl']['_content'], array(
    'attributes' => array(
      'title' => t('View Flickr photostream.'),
      'target' => '_blank',
    ),
    'html' => TRUE,
  ));

  $people['photosurl'] = $info['photosurl']['_content'];
  $people['profileurl'] = $info['profileurl']['_content'];
  $people['count'] = $info['photos']['count']['_content'];
  return $people;
}

/**
 * Media from photoset. Used by blocks 7, 8 and text filter photoset-id.
 */
function _flickr_photoset($photoset_id, $show_n, $size, $media, $tags, $delta = NULL, $random = 0) {
  $cache = cache_get('flickr_block_' . $delta . '_' . $photoset_id . '_' . $tags . '_' . $random);
  // If $delta is a number it is a block that could be cached.
  if ($cache && is_numeric($delta)) {
    $output = $cache->data;
  }
  else {
    switch ($size) {
      case "x":
        $output = theme('flickr_flickrcomslideshow', array(
          'id' => $photoset_id,
        ));
        drupal_set_message(t("Note that slideshows show in reverse chronological order (newest first) and ignore number, tags and media settings."), 'warning');
        break;

      case "y":
        $output = theme('flickr_flickrcomslideshow_simple', array(
          'id' => $photoset_id,
        ));
        drupal_set_message(t("Note that slideshows show in reverse chronological order (newest first) and ignore number, tags and media settings."), 'warning');
        break;

      default:
        $output = '';
        // Get information about the photoset, including the owner.
        $info = flickr_photoset_get_info($photoset_id);
        $people = flickr_get_user_info($info['owner']);
        if (!$info) {
          return;
        }
        // Get a list of "all" the photos in the photoset. This is cached.
        $response = flickr_photosets_getphotos($photoset_id, array(
            'per_page' => 500,
            'extras' => 'license,geo,tags,views',
            'media' => $media,
          ));
        if (!$response) {
          return;
        }
        $photos = $response['photoset']['photo'];
        if ($random == 1) {
          shuffle($photos);
        }
        // We shouldn't try to return more than the total number of photos.
        $to = min($show_n, count($photos));
        $i = 0;
        $j = 0;
        $terms = is_array($tags) ? $tags : explode(",", $tags);
        if (empty($tags)) {
          foreach ($photos as $photo) {
            if ($i < $show_n) {
              // Insert owner into $photo because theme_flickr_photo needs it.
              $photo['owner'] = $info['owner'];
              $output .= theme('flickr_photo', array(
                'photo' => $photo,
                'size' => $size,
              ));
              $i++;
              $j++;
            }
          }
        }
        else {
          foreach ($photos as $key => $photo) {
            foreach ($terms as $term) {
              if ($i >= $show_n && stripos($photo['tags'], $term) !== FALSE) {
                $j++;
              }
              elseif (stripos($photo['tags'], $term) !== FALSE) {
                // Insert owner into $photo because theme_flickr_photo needs it.
                $photo['owner'] = $info['owner'];
                $output .= theme('flickr_photo', array(
                  'photo' => $photo,
                  'size' => $size,
                ));
                // Avoid this image gets repeated by removing it from the
                // array.
                unset($photos[$key]);
                $i++;
                $j++;
              }
            }
          }
        }
        // A counter if set to show.
        // The used tags with a space forced after the comma and the last
        // one replaced by 'or'. In the link all whitespace is removed.
        $flickr = (!empty($tags)) ? l(t("tagged <em>@tag</em>", array(
          '@tag' => preg_replace('/(.*),/', '$1 ' . t('or'), preg_replace("/,([^\s])/", ", $1", $tags)))), $people['photosurl'] . 'tags/' . $tags, array(
            'attributes' => array(
              'title' => t('View tagged photos on Flickr.'),
              'target' => '_blank',
            ),
            'html' => TRUE,
          )) : '';
        $set_title = l($info['title']['_content'], $people['photosurl'] . 'sets/' . $photoset_id, array(
            'attributes' => array(
              'title' => t('View set on Flickr.'),
              'target' => '_blank',
            ),
            'html' => TRUE,
          ));
        $total = (empty($tags)) ? (int) $people['count'] : $j;
        $count = t('@number out of @total in set !set_title !flickr by !user', array(
          '@number' => $i,
          '@total' => number_format($total, 0, ",", "."),
          '!set_title' => $set_title,
          '!flickr' => $flickr,
          '!user' => $people['photostream'],
        ));
        // Note the $delta value is abused for counter override from filter.
        $output .= ((variable_get('flickr_block_counter', 1) && $delta != 'false') || $delta == 'true') ? '<div class="flickr-counter">' . $count . '</div>' : '';
    }
    if (is_numeric($delta)) {
      cache_set('flickr_block_' . $delta . '_' . $photoset_id . '_' . $tags . '_' . $random, $output, 'cache', time() + (variable_get('flickr_block_refresh_random', 23) * 60 * 60));
    }
  }
  return $output;
}

/**
 * User media by tags. Used by blocks 2, 5, 11 and text filter user-id.
 */
function _flickr_user($nsid, $show_n, $size, $media, $tags, $delta, $random = 0) {
  $cache = cache_get('flickr_block_' . $delta . '_' . $nsid . '_' . $tags . '_' . $random);
  // If $delta is a number it is a block that could be cached.
  if ($cache && is_numeric($delta)) {
    $output = $cache->data;
  }
  else {
    switch ($size) {
      case "x":
        $output = theme('flickr_flickrcomslideshow', array(
          'id' => $nsid,
        ));
        drupal_set_message(t("Note that slideshows show in reverse chronological order and ignore number, tags and media settings."), 'warning');
        break;

      case "y":
        $output = theme('flickr_flickrcomslideshow_simple', array(
          'id' => $nsid,
        ));
        drupal_set_message(t("Note that slideshows show in reverse chronological order (newest first) and ignore number, tags and media settings."), 'warning');
        break;

      default:
        $people = flickr_get_user_info($nsid);
        $output = '';
        if ($photos = flickr_photos_search($nsid, 1, array(
          'per_page' => 500,
          'extras' => 'license,geo,tags,views',
          'media' => $media,
        ))) {
          $i = 0;
          $j = 0;
          $response = $photos['photo'];
          if ($random == 1) {
            shuffle($response);
          }
          $terms = is_array($tags) ? $tags : explode(",", $tags);
          if (empty($tags)) {
            foreach ($response as $photo) {
              if ($i < $show_n) {
                $output .= theme('flickr_photo', array(
                  'photo' => $photo,
                  'size' => $size,
                ));
                $i++;
                $j++;
              }
            }
          }
          else {
            foreach ($response as $key => $photo) {
              foreach ($terms as $term) {
                if ($i >= $show_n && stripos($photo['tags'], $term) !== FALSE) {
                  $j++;
                }
                elseif (stripos($photo['tags'], $term) !== FALSE) {
                  $output .= theme('flickr_photo', array(
                    'photo' => $photo,
                    'size' => $size,
                  ));
                  // Avoid this image gets repeated by removing it from the
                  // array.
                  unset($response[$key]);
                  $i++;
                  $j++;
                }
              }
            }
          }
          // A counter if set to show.
          // The used tags with a space forced after the comma and the last
          // one replaced by 'or'. In the link all whitespace is removed.
          $flickr = (!empty($tags)) ? l(t("tagged <em>@tag</em>", array(
            '@tag' => preg_replace('/(.*),/', '$1 ' . t('or'), preg_replace("/,([^\s])/", ", $1", $tags)))), $people['photosurl'] . 'tags/' . $tags, array(
              'attributes' => array(
                'title' => t('View tagged photos on Flickr.'),
                'target' => '_blank',
              ),
              'html' => TRUE,
            )) : '';
          $total = (empty($tags)) ? (int) $people['count'] : $j;
          $count = t('@number out of @total !flickr by !user', array(
            '@number' => $i,
            '@total' => number_format($total, 0, ",", "."),
            '!flickr' => $flickr,
            '!user' => $people['photostream'],
          ));
          // Note the $delta value is abused for counter override from filter.
          $output .= ((variable_get('flickr_block_counter', 1) && $delta != 'false') || $delta == 'true') ? '<div class="flickr-counter">' . $count . '</div>' : '';
        }
        if ($i == 0) {
          $output .= t('No media found in the last 500 posted on Flickr.');
          continue;
        }
    }
    if (is_numeric($delta)) {
      cache_set('flickr_block_' . $delta . '_' . $nsid . '_' . $tags . '_' . $random, $output, 'cache', time() + (variable_get('flickr_block_refresh_random', 23) * 60 * 60));
    }
  }
  return $output;
}

/**
 * Media from group. Used by locks 6, 10 and text filter group-id.
 */
function _flickr_group($group_id, $show_n, $size, $media, $tags, $delta, $total = NULL, $random = 0) {
  $cache = cache_get('flickr_block_' . $delta . '_' . $group_id . '_' . $tags . '_' . $random);
  // If $delta is a number it is a block that could be cached.
  if ($cache && is_numeric($delta)) {
    $output = $cache->data;
  }
  else {
    switch ($size) {
      case "x":
        $output = theme('flickr_flickrcomslideshow', array(
          'id' => $group_id,
        ));
        drupal_set_message(t("Note that slideshows show in reverse chronological order (newest first) and ignore number, tags and media settings."), 'warning');
        break;

      case "y":
        $output = t('Choose the Flash slideshow for group pools (size = x).');
        drupal_set_message(t('Choose the Flash slideshow for group pools (size = x).'), 'error');
        break;

      default:
        $info = flickr_groups_getinfo($group_id);
        $output = '';
        // Get a list of "all" the photos in the group. This is cached.
        if ($photos = flickr_get_group_photos($group_id, 1, array(
          'per_page' => 500,
        ))) {
          $i = 0;
          $j = 0;
          $response = $photos['photos']['photo'];
          if ($random == 1) {
            shuffle($response);
          }
          $terms = is_array($tags) ? $tags : explode(",", $tags);
          if (empty($tags)) {
            foreach ($response as $photo) {
              if ($i < $show_n && ($media == 'all' || $media == $photo['media'] . 's')) {
                $output .= theme('flickr_photo', array(
                  'photo' => $photo,
                  'size' => $size,
                ));
                $i++;
                $j++;
              }
              else {
                $j++;
              }
            }
          }
          else {
            foreach ($response as $key => $photo) {
              foreach ($terms as $term) {
                if ($i >= $show_n && stripos($photo['tags'], $term) !== FALSE) {
                  $j++;
                }
                elseif (stripos($photo['tags'], $term) !== FALSE && ($media == 'all' || $media == $photo['media'] . 's')) {
                  $output .= theme('flickr_photo', array(
                    'photo' => $photo,
                    'size' => $size,
                  ));
                  // Avoid this image gets repeated by removing it from the
                  // array.
                  unset($response[$key]);
                  $i++;
                  $j++;
                }
              }
            }
          }
          // A counter if set to show.
          // The used tags with a space forced after the comma and the last
          // one replaced by 'or'. In the link all whitespace is removed.
          $tagged = (!empty($tags)) ? t("tagged <em>@tag</em>", array(
            '@tag' => preg_replace('/(.*),/', '$1 ' . t('or'), preg_replace("/,([^\s])/", ", $1", $tags)))) . ' ' : '';
          $flickr = l($info['group']['name']['_content'], 'https://www.flickr.com/groups/' . $group_id, array(
            'attributes' => array(
              'title' => t('View group on Flickr.'),
              'target' => '_blank',
            ),
            'html' => TRUE,
          ));
          $total = (empty($tags)) ? (int) $info['group']['pool_count']['_content'] : $j;
          $count = t('@number out of @total !tagsfrom the Flickr group !flickr', array(
            '@number' => $i,
            '@total' => number_format($total, 0, ",", "."),
            '!tags' => $tagged,
            '!flickr' => $flickr,
          ));
          // Note the $delta value is abused for counter override from filter.
          $output .= ((variable_get('flickr_block_counter', 1) && $delta != 'false') || $delta == 'true') ? '<div class="flickr-counter">' . $count . '</div>' : '';
        }
        if ($i == 0) {
          $output .= t('No media found in the last @count posted on Flickr.', array('@count' => count($photos)));
          continue;
        }
    }
    if (is_numeric($delta)) {
      cache_set('flickr_block_' . $delta . '_' . $group_id . '_' . $tags . '_' . $random, $output, 'cache', time() + (variable_get('flickr_block_refresh_random', 23) * 60 * 60));
    }
  }
  return $output;
}
