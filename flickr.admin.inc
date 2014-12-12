<?php
/**
 * @file
 * The admin settings for the Flickr module
 */

/**
 * Implements hook_settings().
 */
function flickr_admin_settings() {
  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $apply = l(t('https://www.flickr.com/services/apps/create/apply'), 'https://www.flickr.com/services/apps/create/apply', array(
      'attributes' => array(
        'title' => t('Flickr Services'),
        'target' => '_blank',
      ),
  ));
  $form['credentials']['flickr_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_api_key', ''),
    '#description' => t("API Key from Flickr. Get an API Key at !apply.", array('!apply' => $apply)),
  );
  $form['credentials']['flickr_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('API Shared Secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_api_secret', ''),
    '#description' => t("API key's secret from Flickr."),
  );
  $default = variable_get('flickr_default_userid', '');
  if (!empty($default)) {
    $info = flickr_people_getinfo($default);
    $default = $info['username']['_content'];
  }
  $form['credentials']['flickr_default_userid'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Flickr User ID'),
    '#default_value' => $default,
    '#description' => t('An, optional, default Flickr user (number@number, alias, username or email). This will be used when no user is specified.'),
  );
  $form['info_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global display options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['info_settings']['flickr_photos_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of photos per album'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_photos_per_page', 6),
    '#description' => t('The number indicates how many photos of a photoset display in your nodes if no number is specified. Clear the cache on form submit.'),
    '#size' => 3,
    '#maxlength' => 3,
  );
  $form['info_settings']['flickr_default_size_album'] = array(
    '#type' => 'select',
    '#title' => t('Default size for photos in an album'),
    '#default_value' => variable_get('flickr_default_size_album', 's'),
    '#options' => array(
      's' => t('s: 75 px square'),
      't' => t('t: 100 px on longest side'),
      'q' => t('q: 150 px square'),
      'm' => t('m: 240 px on longest side'),
      'n' => t('n: 320 px on longest side (!)'),
      '-' => t('-: 500 px on longest side'),
      'z' => t('z: 640 px on longest side'),
      'c' => t('c: 800 px on longest side (!)'),
      'b' => t('b: 1024 px on longest side'),
    ),
    '#description' => t('A default Flickr size to use if no size is specified, for example [flickr-photoset:id=72157634563269642].<br />Clear the cache on form submit.<br />!: TAKE CARE, n (320px) and c (800px) sizes are missing on many "older" Flickr images!'),
  );
  $guidelines = l(t('Guidelines'), 'https://www.flickr.com/guidelines.gne/', array(
      'attributes' => array(
        'title' => t('Flickr - Community Guidelines'),
        'target' => '_blank',
      ),
  ));
  $attribution = l(t('proper attribution'), 'https://www.flickr.com/services/developer/attributions/', array(
      'attributes' => array(
        'title' => t('The Flickr Developer Guide - Attributions'),
        'target' => '_blank',
      ),
  ));
  $form['info_settings']['flickr_title_suppress_on_small'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum image width to display a title caption'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_title_suppress_on_small', '100'),
    '#description' => t("Small images have liitle space for a title caption. Replace it with the text 'Flickr' that links to the photo page on Flickr to comply with their !guidelines.<br />Set it to '0 px' to always include or '999 px' to always exclude. To give !attribution this should be included (space allowing). Clear the cache on form submit.", array('!attribution' => $attribution, '!guidelines' => $guidelines)),
    '#field_suffix' => t('px'),
    '#size' => 3,
    '#maxlength' => 3,
  );
  $form['info_settings']['flickr_metadata_suppress_on_small'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum image width to display date, location, photographer and optionally license info under the caption'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_metadata_suppress_on_small', '150'),
    '#description' => t("Suppress extra info on small images. Set it to '0 px' to always include or '999 px' to always exclude. To give !attribution this should be included (space allowing). Clear the cache on form submit.", array('!attribution' => $attribution)),
    '#field_suffix' => t('px'),
    '#size' => 3,
    '#maxlength' => 3,
  );
  $cc_icons = l(t('Source for the CC icon font'), 'https://cc-icons.github.io/', array(
      'attributes' => array(
        'title' => t('Creative Commons Icon Font on GitHub'),
        'target' => '_blank',
      ),
  ));
  $cc_example_cna = '<span class="flickr-cc">' . l('cna', 'https://creativecommons.org/licenses/by-nc-sa/2.0/', array(
    'attributes' => array(
      'title' => t('Creative Commons — Attribution-NonCommercial-ShareAlike 2.0 Generic — CC BY-NC-SA 2.0'),
      'target' => '_blank',
    ),
  )) . '</span>';
  $cc_example_copy = '<span class="flickr-copyright">' . l('©', 'https://en.wikipedia.org/wiki/Copyright', array(
    'attributes' => array(
      'title' => t('All Rights Reserved'),
      'target' => '_blank',
    ),
    'html' => TRUE,
  )) . '</span>';
  $cc_example_p = '<span class="flickr-cc">' . l('p', 'https://flickr.com/commons/usage/', array(
    'attributes' => array(
      'title' => t('Public Domain - No known copyright restrictions'),
      'target' => '_blank',
    ),
  )) . '</span>';
  $form['info_settings']['flickr_license'] = array(
    '#type' => 'checkbox',
    '#title' => t('License info in caption'),
    '#default_value' => variable_get('flickr_license', 0),
    '#description' => t("To give !attribution. Depends on 'Minimum image width' above. Clear the cache on form submit.<br />!cc_icons in 'flickr_cc_icons.css'. Download it to host yourself locally (CC 4.0 licensed). Adjust 'flickr_cc_icons.css' accordingly.<br />Some examples (try to mouse-over): !ccexample_cna !cc_example_copy !cc_example_p", array('!attribution' => $attribution, '!cc_icons' => $cc_icons, '!ccexample_cna' => $cc_example_cna, '!cc_example_copy' => $cc_example_copy,'!cc_example_p' => $cc_example_p)),
  );
  $form['info_settings']['flickr_maps'] = array(
    '#type' => 'checkbox',
    '#title' => t('Extra links to Flickr maps'),
    '#default_value' => variable_get('flickr_maps', 0),
    '#description' => t('Include extra links to maps available for a user, group or set on Flickr. Locations mentioned (if displayed) under individual images link to corresponding Flickr user maps in any case, independent on the setting here.'),
  );
  $form['info_settings']['flickr_counter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a Flickr counter'),
    '#default_value' => variable_get('flickr_counter', 1),
    '#description' => t('Shows how many photos are displayed out of the total number available for a user, group, set or tags on Flickr. Can be overridden by the filter tag, eg. [flickr-photoset:id=72157634563269642,count=false]'),
  );
  $colorbox_module = l(t('Colorbox module'), 'https://drupal.org/project/colorbox', array(
      'attributes' => array(
        'title' => t('Colorbox| Drupal.org'),
        'target' => '_blank',
      ),
  ));
  $form['overlay_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay browser (Colorbox, Lightbox)'),
    '#description' => t('Recommended is the !colorbox_module. Leave empty to link directly to the Flickr photo page instead of opening the bigger version of the image.', array('!colorbox_module' => $colorbox_module)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['overlay_settings']['flickr_class'] = array(
    '#type' => 'textfield',
    '#title' => t('class'),
    '#default_value' => variable_get('flickr_class', ''),
    '#description' => t('For example: <em>colorbox</em>. Can be left empty for Lightbox. Clear the cache on form submit.'),
  );
  $form['overlay_settings']['flickr_rel'] = array(
    '#type' => 'textfield',
    '#title' => t('rel'),
    '#default_value' => variable_get('flickr_rel', ''),
    '#description' => t('For example: <em>gallery-all</em> for Colorbox or <em>lightbox[gallery]</em>. Clear the cache on form submit.'),
  );
  $form['overlay_settings']['flickr_opening_size'] = array(
    '#type' => 'select',
    '#title' => t('Image size to open'),
    '#default_value' => variable_get('flickr_opening_size', ''),
    '#options' => array(
      'q' => t('q: 150 px square'),
      'm' => t('m: 240 px on longest side'),
      'n' => t('n: 320 px on longest side (!)'),
      '' => t('-: 500 px on longest side'),
      'z' => t('z: 640 px on longest side'),
      'c' => t('c: 800 px on longest side (!)'),
      'b' => t('b: 1024 px on longest side'),
      'h' => t('h: 1600 px on longest side'),
    ),
    '#description' => t('The image size to open in the overlay browser when clicking the image. Larger sizes make navigating to next and previous pictures slower.<br />Clear the cache on form submit.<br />!: TAKE CARE, n (320px) and c (800px) sizes are missing on many "older" Flickr images!'),
  );
  $form['overlay_settings']['flickr_info_overlay'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Info to include when enlarging the image'),
    '#default_value' => variable_get('flickr_info_overlay', array(
      'title' => 'title',
      'metadata' => 'metadata',
      'description' => 'description',
      'license' => 0,
    )),
    '#description' => t("To give !attribution all marked * should be checked. Clear the cache on form submit.", array('!attribution' => $attribution)),
    '#options' => array(
      'title' => t('Title *'),
      'metadata' => t('Date, location and photographer *'),
      'description' => t("Description, applies also on the text that shows on mouseover (the image 'title' attribute)"),
      'license' => t('License info *'),
    ),
  );
  $form['css_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Styling (CSS related)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $style_doc = l(t('Flickr module Style Guide'), 'https://drupal.org/node/2174131', array(
      'attributes' => array(
        'title' => t('Flickr Style Guide documentation | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  $autofloat_module = l(t('AutoFloat module'), 'https://drupal.org/project/autofloat', array(
      'attributes' => array(
        'title' => t('AutoFloat | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  $form['css_settings']['flickr_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use flickr.css'),
    '#default_value' => variable_get('flickr_css', 1),
    '#description' => t("Uncheck to take care of the styling yourself in custom CSS.<br />Side note: Extend CSS even further with the !style_doc. If you use Flickr Filter, you might find the !autofloat_module useful.", array('!style_doc' => $style_doc, '!autofloat_module' => $autofloat_module)),
  );
    $style_doc = l(t('Style Guide'), 'https://drupal.org/node/2174131#caption', array(
      'attributes' => array(
        'title' => t('Flickr Style Guide documentation | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  $form['css_settings']['flickr_caption_padding'] = array(
    '#type' => 'textfield',
    '#title' => t('Make the caption smaller in width (correction for caption padding/border)'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_caption_padding', '0'),
    '#description' => t('By default the caption is as wide as the image. If you add padding to a boxed caption with CSS, set a value here to make sure the caption will remain of the same width of the image. See the !style_doc.<br />Only padding: 1 x horizontal padding value.<br />Padding AND border: 2 x padding value + 1 x border-width.', array('!style_doc' => $style_doc)),
    '#field_suffix' => t('px'),
    '#size' => 2,
    '#maxlength' => 2,
  );
$form['advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $times = array(
    900,
    1800,
    2700,
    3600,
    7200,
    10800,
    14400,
    18000,
    21600,
    43200,
    86400,
  );
  $ageoptions = drupal_map_assoc($times, 'format_interval');
  $form['advanced_settings']['flickr_cache_duration'] = array(
    '#type' => 'select',
    '#title' => t('Update interval'),
    '#options' => $ageoptions,
    '#default_value' => variable_get('flickr_cache_duration', 3600),
    '#description' => t('The refresh interval indicating how often you want to check cached Flickr API calls are up to date.'),
  );
  $cache_warming = l(t('cache warming'), 'https://drupal.org/node/1576686', array(
    'attributes' => array(
      'title' => t('Load Page Cache after cron Runs | Drupal.org'),
      'target' => '_blank',
    ),
  ));
  $form['advanced_settings']['flickr_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit the number of photos to grab for random and popularity sort'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_per_page', 100),
    '#description' => t('Setting a lower number enhances performance but makes random results being less spread between one another (not less random) and returns popular (most viewed on Flickr) only from the <em>n</em> most recent.<br />Minimum 20, maximum 500. Set the maximum only if you use !cache_warming.', array('!cache_warming' => $cache_warming)),
    '#size' => 3,
    '#maxlength' => 3,
  );
  if (!module_exists('devel')) {
    variable_set('flickr_debug', 0);
  }
  $devel_module = l(t('Devel module'), 'https://drupal.org/project/devel', array(
      'attributes' => array(
        'title' => t('Devel | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  $form['advanced_settings']['flickr_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Debug Output'),
    '#description' => t('Display all passed photo/album arguments and HTTP requests/response objects via the !devel_module.', array('!devel_module' => $devel_module)),
    '#default_value' => variable_get('flickr_debug', 0),
  );
  $more_info = l(t('More info'), 'https://stackoverflow.com/a/4635991', array(
      'attributes' => array(
        'title' => t("Super fast 'getimagesize' in PHP - Stack Overflow"),
        'target' => '_blank',
      ),
  ));
  $form['advanced_settings']['flickr_curl'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use 'cURL' to determine the image width instead of 'fopen' used by the PHP function 'getimagesize'."),
    '#default_value' => variable_get('flickr_curl', 0),
    '#description' => t("Your server configuration now uses 'fopen' for external resources (used by 'getimagesize'). 'cURL' might be faster. !more_info.", array('!more_info' => $more_info)),
  );
  $form['advanced_settings']['flickr_curl2'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use 'cURL' instead of 'stream_socket_client' (drupal_http_request) to make Flickr data requests."),
    '#default_value' => variable_get('flickr_curl2', 0),
    '#description' => t("Otherwise cURL will only be used as fallback when the usual method returns a 403 (forbidden) or 408 (Request Timeout)."),
  );
   // We need an api key before we can verify usernames.
  if (!$form['credentials']['flickr_api_key']['#default_value']) {
    $form['credentials']['flickr_default_userid']['#disabled'] = TRUE;
    $form['credentials']['flickr_default_userid']['#description'] .= ' ' . t('Disabled until a valid API Key is set.');
  }
    // Do not offer the following setting if Devel is not available.
  if (!module_exists('devel')) {
    $form['advanced_settings']['flickr_debug']['#disabled'] = TRUE;
    $form['advanced_settings']['flickr_debug']['#description'] = t("Disabled because the !devel_module is not enabled.", array('!devel_module' => $devel_module));
  }
  // Offer the following setting if both allow_url_fopen and curl are enabled.
  if (!ini_get("allow_url_fopen") || !function_exists('curl_version')) {
    $form['advanced_settings']['flickr_curl']['#disabled'] = TRUE;
    if (function_exists('curl_version')) {
      $form['advanced_settings']['flickr_curl']['#description'] = t("Disabled because your server configuration only uses 'cURL' (not 'fopen')");
    }
    elseif (ini_get("allow_url_fopen")) {
      $form['advanced_settings']['flickr_curl']['#description'] = t("Disabled because your server configuration only uses 'fopen' (not 'cURL')");
    }
    else {
      $form['advanced_settings']['flickr_curl']['#description'] = t("It could not be determined if your server configuration uses 'fopen' or 'cURL'. You might see unnecessary whitespace next to your floating images. It probably means your server does not allow neither 'fopen' nor 'cURL'. Check your 'php.ini' settings first, then contact your hosting company.");
    }
  }
    // Do not offer the following setting if curl is not available.
  if (!function_exists('curl_version')) {
    $form['advanced_settings']['flickr_curl2']['#disabled'] = TRUE;
    $form['advanced_settings']['flickr_curl2']['#description'] = t("Disabled because your server configuration only allows 'stream_socket_client' (not 'cURL')");
  }
  if (module_exists('flickr_filter')) {
    $form['filter_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Filter options'),
      '#description' => t('Clear the cache on form submit.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['filter_settings']['flickr_filter_default_size'] = array(
      '#type' => 'select',
      '#title' => t('Default size for single photos'),
      '#default_value' => variable_get('flickr_filter_default_size', 'm'),
      '#options' => array(
        's' => t('s: 75 px square'),
        't' => t('t: 100 px on longest side'),
        'q' => t('q: 150 px square'),
        'm' => t('m: 240 px on longest side'),
        'n' => t('n: 320 px on longest side (!)'),
        '-' => t('-: 500 px on longest side'),
        'z' => t('z: 640 px on longest side'),
        'c' => t('c: 800 px on longest side (!)'),
        'b' => t('b: 1024 px on longest side'),
      ),
      '#description' => t('A default Flickr size to use if no size is specified, for example [flickr-photo:id=3711935987].<br />Clear the cache on form submit.<br />!: TAKE CARE, n (320px) and c (800px) sizes are missing on many "older" Flickr images!'),
    );
    $form['filter_settings']['flickr_filter_heading'] = array(
      '#type' => 'textfield',
      '#title' => t('Wrap the album title in an HTML heading tag (only for the text filter)'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_filter_heading', 'p'),
      '#description' => t("Use 'p' for no style, e.g. 'h3' for a heading or 'none' to not display an album title."),
      '#size' => 4,
      '#maxlength' => 4,
    );
    // Offer the following setting if the ctools module is enabled.
    if (module_exists('ctools')) {
      $form['filter_settings']['flickr_filter_collapsible'] = array(
        '#type' => 'checkbox',
        '#title' => t("Collapsible filter tips (collapsed by default)"),
        '#default_value' => variable_get('flickr_filter_collapsible', 1),
        '#description' => t('Collapse the in-line help for the Flickr Filter text format under the body field on node edit forms.'),
      );
    }
  }
  if (module_exists('flickr_block')) {
    $form['block_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Block options'),
      '#description' => t('Clear the cache on form submit.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['block_settings']['flickr_block_hide_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide empty blocks (no media found)'),
      '#default_value' => variable_get('flickr_block_hide_empty', 0),
      '#description' => t("Uncheck to see the block title and to access the contextual links. That little wheel you see when you hover over blocks to edit them in place."),
    );
    $form['block_settings']['flickr_block_heading'] = array(
      '#type' => 'textfield',
      '#title' => t('Wrap the album title in an HTML heading tag (only for blocks)'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_block_heading', 'h2'),
      '#description' => t("Use 'p' for no style, e.g. 'h2' for a heading or 'none' to not display an album title."),
      '#size' => 4,
      '#maxlength' => 4,
    );
    $form['block_settings']['flickr_block_refresh_random'] = array(
      '#type' => 'textfield',
      '#field_prefix' => t('Refresh random blocks every'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_block_refresh_random', 23),
      '#field_suffix' => t('hours.'),
      '#size' => 3,
      '#maxlength' => 3,
    );
  $cache_warming = l(t('cache warming'), 'https://drupal.org/node/1576686/', array(
      'attributes' => array(
        'title' => t('Load Page Cache after cron Runs | Drupal.org'),
        'target' => '_blank',
      ),
  ));
    $form['block_settings']['flickr_block_refresh_others'] = array(
      '#type' => 'textfield',
      '#field_prefix' => t('Refresh other blocks every'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_block_refresh_others', 31),
      '#description' => t('It is recommended to set two different refresh values to avoid all blocks refresh together. Lower values impact performance negatively. Consider to use !cache_warming.', array('!cache_warming' => $cache_warming)),
      '#field_suffix' => t('hours.'),
      '#size' => 3,
      '#maxlength' => 3,
    );
    $form['block_settings']['flickr_block_photos_per_set'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of photos per set'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_block_photos_per_set', 6),
      '#description' => t('The number indicates how many photos display per set on blocks that display multiple photosets. Clear the cache on form submit.'),
      '#size' => 2,
      '#maxlength' => 2,
    );
  }
  if (module_exists('flickr_sets')) {
    $form['photoset_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Photoset options'),
      '#description' => t('Clear the cache on form submit.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['photoset_settings']['flickr_sets_thumb_default_size'] = array(
      '#type' => 'select',
      '#title' => t('Size for the photo representing a set'),
      '#default_value' => variable_get('flickr_sets_thumb_default_size', 's'),
      '#options' => array(
        's' => t('s: 75 px square'),
        't' => t('t: 100 px on longest side'),
        'q' => t('q: 150 px square'),
        'm' => t('m: 240 px on longest side'),
        'n' => t('n: 320 px on longest side (!)'),
        '-' => t('-: 500 px on longest side'),
        'z' => t('z: 640 px on longest side'),
        'c' => t('c: 800 px on longest side (!)'),
        'b' => t('b: 1024 px on longest side'),
      ),
      '#description' => t('!: TAKE CARE, n (320px) and c (800px) sizes are missing on many "older" Flickr images!'),
    );
    $form['photoset_settings']['flickr_sets_per_page'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of photosets per page'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_sets_per_page', 8),
      '#description' => t('The number indicates how many photosets display on user profile pages at <em>flickr/%uid/sets</em>. Clear the cache on form submit.'),
      '#size' => 2,
      '#maxlength' => 2,
    );
    $form['photoset_settings']['flickr_sets_photos_per_set'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of photos per set'),
      '#required' => TRUE,
      '#default_value' => variable_get('flickr_sets_photos_per_set', 6),
      '#description' => t('The number indicates how many photos display in a set on user profile pages at <em>flickr/%uid/sets/%nsid</em>. Clear the cache on form submit.'),
      '#size' => 2,
      '#maxlength' => 2,
    );
  }
  $form['flickr_cc'] = array(
    '#type' => 'checkbox',
    '#title' => t("Flush all caches on form submit to see your changes instantly."),
    '#default_value' => variable_get('flickr_cc', 0),
    '#description' => t("Note that form submit will be slower and all your site content will be rebuilt. Your choice will be 'remembered' for your next visit to this configuration page."),
  );

  // Call submit_function() on form submission.
  $form['#submit'][] = 'flickr_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate user input.
 */
function flickr_admin_settings_validate($form, &$form_state) {
  $key = trim($form_state['values']['flickr_api_key']);
  $sec = trim($form_state['values']['flickr_api_secret']);
  $uid = trim($form_state['values']['flickr_default_userid']);
  $limit1 = trim($form_state['values']['flickr_photos_per_page']);
  $limit2 = trim($form_state['values']['flickr_title_suppress_on_small']);
  $limit3 = trim($form_state['values']['flickr_metadata_suppress_on_small']);
  $limit4 = trim($form_state['values']['flickr_caption_padding']);
  $limit7 = trim($form_state['values']['flickr_per_page']);
  if (module_exists('flickr_block')) {
    $limit5 = trim($form_state['values']['flickr_block_refresh_random']);
    $limit6 = trim($form_state['values']['flickr_block_refresh_others']);
    $limit10 = trim($form_state['values']['flickr_block_photos_per_set']);
  }
  if (module_exists('flickr_sets')) {
    $limit8 = trim($form_state['values']['flickr_sets_per_page']);
    $limit9 = trim($form_state['values']['flickr_sets_photos_per_set']);
  }
  if ($key && (preg_match('/^[A-Fa-f\d]{32}$/', $key) != 1)) {
    form_set_error('flickr_api_key', t('This does not appear to be a Flickr API key.'));
  }
  if ($sec && (preg_match('/^[A-Fa-f\d]{16}$/', $sec) != 1)) {
    form_set_error('flickr_api_secret', t('This does not appear to be a Flickr API secret.'));
  }
  if ($uid) {
    $user = flickr_user_find_by_identifier($uid);
    if (!$user) {
      form_set_error('flickr_default_userid', t('%uid does not appear to be a valid Flickr user.', array('%uid' => $uid)));
    }
  }
  // Validate the number of photos.
  if (!is_numeric($limit1) || $limit1 < 1) {
    form_set_error('flickr_photos_per_page', t('Set a number from 1 to 999.'));
  }
  // Validate the minimum width to suppress title caption.
  if (!is_numeric($limit2) || $limit2 < 0) {
    form_set_error('flickr_title_suppress_on_small', t('Set a width from 0 to 999 px.'));
  }
  // Validate the minimum width to suppress metadata caption.
  if (!is_numeric($limit3) || $limit3 < 0) {
    form_set_error('flickr_metadata_suppress_on_small', t('Set a width from 0 to 999 px.'));
  }
  // Validate the padding correction for caption width.
  if (!is_numeric($limit4) || $limit4 < 0) {
    form_set_error('flickr_caption_padding', t('Set a padding from 0 to 99 px.'));
  }
  // Validate the number to return on a random or views sorted API requests.
  if (!is_numeric($limit7) || $limit7 < 20 || $limit7 > 500) {
    form_set_error('flickr_per_page', t('Set a number from 20 to 500.'));
  }
  if (module_exists('flickr_block')) {
    // Validate refresh rate random blocks.
    if (!is_numeric($limit5) || $limit5 < 2) {
      form_set_error('flickr_block_refresh_random', t('Set a number from 2 to 999.'));
    }
    // Validate refresh rate other blocks.
    if (!is_numeric($limit6) || $limit6 < 2) {
      form_set_error('flickr_block_refresh_others', t('Set a number from 2 to 999.'));
    }
    // Validate photos per set.
    if (!is_numeric($limit10) || $limit10 < 1) {
      form_set_error('flickr_block_photos_per_set', t('Set a number from 1 to 99.'));
    }
  }
  if (module_exists('flickr_sets')) {
    // Validate sets per page.
    if (!is_numeric($limit8) || $limit8 < 1) {
      form_set_error('flickr_sets_per_page', t('Set a number from 1 to 99.'));
    }
    // Validate photos per set.
    if (!is_numeric($limit9) || $limit9 < 1) {
      form_set_error('flickr_sets_photos_per_set', t('Set a number from 1 to 99.'));
    }
  }
}

/**
 * Submit form data.
 */
function flickr_admin_settings_submit($form, &$form_state) {
  if ($form_state['values']['flickr_cc']) {
    drupal_flush_all_caches();
    drupal_set_message(t('All caches are flushed.'), 'status', FALSE);
  }
  // Clean up the data.
  $form_state['values']['flickr_api_key'] = trim($form_state['values']['flickr_api_key']);
  $form_state['values']['flickr_api_secret'] = trim($form_state['values']['flickr_api_secret']);
  $form_state['values']['flickr_photos_per_page'] = trim($form_state['values']['flickr_photos_per_page']);
  $form_state['values']['flickr_default_userid'] = trim($form_state['values']['flickr_default_userid']);

  // Replace the usernames with a uid.
  // As emails or usernames might change, replace them with a unique nsid.
  if (!flickr_is_nsid($form_state['values']['flickr_default_userid'])) {
    $userid = $form_state['values']['flickr_default_userid'];
    if (empty($userid)) {
      return;
    }
    if ($user = flickr_user_find_by_identifier($userid)) {
      drupal_set_message(t("The Flickr user associated with '%userid' has internally been replaced with the corresponding Flickr ID '%uid'.", array('%userid' => $form_state['values']['flickr_default_userid'], '%uid' => $user)));
      $form_state['values']['flickr_default_userid'] = $user;
    }
  }
  else {
    $info = flickr_people_getinfo($form_state['values']['flickr_default_userid']);
    drupal_set_message(t("The Flickr user associated with '%uid' will be shown to you as Flickr user '%userid'.", array('%uid' => $form_state['values']['flickr_default_userid'], '%userid' => $info['username']['_content'])));
  }
}
