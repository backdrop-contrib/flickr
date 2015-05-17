<?php
/**
 * @file
 * The admin settings for the Flickr module
 */

/**
 * Implements hook_settings().
 */
function flickr_admin_settings() {
  drupal_add_css(drupal_get_path('module', 'flickr') . '/flickr_cc_icons.css', array(
    'group' => CSS_DEFAULT,
    'every_page' => FALSE,
  ));
  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Flickr credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 2,
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
  // We need an api key before we can verify usernames.
  if (!$form['credentials']['flickr_api_key']['#default_value']) {
    $form['credentials']['flickr_default_userid']['#disabled'] = TRUE;
    $form['credentials']['flickr_default_userid']['#description'] .= ' ' . t('Disabled until a valid API Key is set.');
  }
  $form['info_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 4,
  );
  $form['info_settings']['flickr_photos_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of photos per album'),
    '#required' => TRUE,
    '#default_value' => variable_get('flickr_photos_per_page', 6),
    '#description' => t('How many photos of a photoset display in your nodes if no number is specified. Clear the cache on form submit.'),
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
    '#description' => t("A default Flickr size to use if no size is specified, for example [flickr-photoset:id=72157634563269642].<br />Clear the cache on form submit.<br />!: TAKE CARE, the 'c' size (800px) is missing on Flickr images uploaded before March 1, 2012!"),
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
    '#type' => 'radios',
    '#title' => t("License icon"),
    '#options' => array(
      t("No"),
      t("On the image on mouse-over only (small in the top left corner, on hover). NOTE: Does not display with the Flickr Style 'Enlarge'."),
      t("On the image (small in the top left corner, always)"),
      t("In the caption"),
    ),
    '#default_value' => variable_get('flickr_license', 1),
    '#description' => t("To give !attribution. Clear the cache on form submit.<br />!cc_icons in 'flickr_cc_icons.css'. Download it if you prefer to host it yourself locally (CC 4.0 licensed, give credit somewhere). Adjust 'flickr_cc_icons.css' accordingly.<br />Some examples (try to mouse-over): !ccexample_cna !cc_example_copy !cc_example_p", array(
        '!attribution' => $attribution,
        '!cc_icons' => $cc_icons,
        '!ccexample_cna' => $cc_example_cna,
        '!cc_example_copy' => $cc_example_copy,
        '!cc_example_p' => $cc_example_p,
      )),
  );
  $form['info_settings']['flickr_restrict'] = array(
    '#type' => 'radios',
    '#title' => t("License restriction for 'public' queries"),
    '#options' => array(
      t("Always restrict 'public' queries to only Creative Commons licensed media."),
      t("Do not restrict media to Creative Commons licensed on 'public' queries if no results are returned."),
      t("Do not restrict media to Creative Commons licensed on 'public' queries."),
    ),
    '#default_value' => variable_get('flickr_restrict', 1),
    '#description' => t("With 'public' queries are intended Flickr requests that do not specify a Flickr user or group ID, thus returning results from all public Flickr photos."),
  );
  $form['info_settings']['flickr_extend'] = array(
    '#type' => 'checkbox',
    '#title' => t("Extend the tag filter to search for matching terms also in the Flickr photo title and description besides Flickr tags. Descriptions are only searched on the album type 'user' (also 'public')."),
    '#default_value' => variable_get('flickr_extend', 1),
    '#description' => t("Shows more results. Can be overridden individually by the filter tag, eg. [flickr-user:id=public, size=q, tags=Augusto Canario, extend=true] or in the specific configuration of a Flickr block."),
  );
  $form['info_settings']['flickr_maps'] = array(
    '#type' => 'checkbox',
    '#title' => t('Extra links to Flickr maps'),
    '#default_value' => variable_get('flickr_maps', 0),
    '#description' => t('Include extra links to maps available for a user, group or set on Flickr. Locations mentioned (if displayed) under individual images link to corresponding Flickr user maps in any case, independent of the setting here.'),
  );
  $form['info_settings']['flickr_counter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a Flickr counter'),
    '#default_value' => variable_get('flickr_counter', 1),
    '#description' => t('Shows how many photos are displayed out of the total number available for a user, group, set or tags on Flickr. Can be overridden individually by the filter tag, eg. [flickr-photoset:id=72157634563269642,count=false]'),
  );
  $form['info_settings']['flickr_geophp'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Use Google instead of Flickr for location info (reverse geocoding)'),
    '#default_value' => variable_get('flickr_geophp', array(
      'title' => 'title',
      'caption' => 0,
    )),
    '#options' => array(
      'title' => t('In the album title'),
      'caption' => t('In the photo caption'),
    ),
  );
  $form['date_formats_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date formats'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 6,
  );
  // Get list of all available date formats.
  drupal_static_reset('system_get_date_types');
  $formats = array();
  foreach (system_get_date_types() as $f => $format) {
    if (($format_string = variable_get('date_format_' . $f, FALSE)) === FALSE) {
      $format_string = key(system_get_date_formats($f));
    }

    if (!empty($format_string)) {
      $formats[$f] = $format['title'] . ' [' . format_date(REQUEST_TIME, 'custom', $format_string) . ']';
    }
  }
  $formats['interval'] = 'Time ago [' . format_interval(3600 * 24 * 90, 1) . ' ago]';
  $form['date_formats_settings']['flickr_date_format_image_title'] = array(
    '#type' => 'select',
    '#title' => t('When hovering an image (mouse-over)'),
    '#default_value' => variable_get('flickr_date_format_image_title', 'medium'),
    '#options' => $formats,
    '#empty_option' => t('- None -'),
    '#empty_value' => 'none',
  );
  $form['date_formats_settings']['flickr_date_format_image_caption'] = array(
    '#type' => 'select',
    '#title' => t('In the image caption'),
    '#default_value' => variable_get('flickr_date_format_image_caption', 'interval'),
    '#options' => $formats,
    '#empty_option' => t('- None -'),
    '#empty_value' => 'none',
  );
  $form['date_formats_settings']['flickr_date_format_image_caption_hover'] = array(
    '#type' => 'select',
    '#title' => t('When hovering a date in the caption'),
    '#description' => t("If you don't want to display anything when hovering the date, select 'None'."),
    '#default_value' => variable_get('flickr_date_format_image_caption_hover', 'long'),
    '#options' => $formats,
    '#empty_option' => t('- None -'),
    '#empty_value' => 'none',
  );
  // Disable the caption hover option if a date in the caption is set to 'none'.
  if (variable_get('flickr_date_format_image_caption', 'interval') == 'none') {
    $form['date_formats_settings']['flickr_date_format_image_caption_hover']['#disabled'] = TRUE;
    $form['date_formats_settings']['flickr_date_format_image_caption_hover']['#title'] = '<span class="grayed-out">' . t('When hovering a date in the caption') . '</span>';
    $form['date_formats_settings']['flickr_date_format_image_caption_hover']['#description'] = t('Disabled until a date format for the image caption is selected.');
  }
  $form['date_formats_settings']['flickr_date_format_album_title'] = array(
    '#type' => 'select',
    '#title' => t('In the album title'),
    '#description' => t("If the selected date format contains a time, only the date part of it will be used in the album title."),
    '#default_value' => variable_get('flickr_date_format_album_title', 'short'),
    '#options' => $formats,
    '#empty_option' => t('- None -'),
    '#empty_value' => 'none',
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
    '#weight' => 8,
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
    '#description' => t("The image size to open in the overlay browser when clicking the image. Larger sizes make navigating to next and previous pictures slower.<br />Clear the cache on form submit.<br />!: TAKE CARE, the 'c' size (800px) is missing on Flickr images uploaded before March 1, 2012!"),
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
    '#weight' => 10,
  );
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
    '#description' => t("Uncheck to take care of the styling yourself in custom CSS.<br />If you use Flickr Filter, you might find the !autofloat_module useful.", array('!autofloat_module' => $autofloat_module)),
  );
  $flickr_style = l(t('Flickr Style'), 'admin/modules', array(
      'attributes' => array(
        'title' => t('Extra configuration options.'),
      ),
  ));
  if (!module_exists('flickrstyle')) {
    $form['css_settings']['flickr_style'] = array(
      '#markup' => t("Extend the styling options with rounded corners, shadow, border and emphasize on hover by enabling the !flickr_style sub-module.", array('!flickr_style' => $flickr_style)),
    );
  }
  $form['advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 12,
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
    '#title' => t("Use 'cURL' instead of 'stream_socket_client' (drupal_http_request) to make data requests."),
    '#default_value' => variable_get('flickr_curl2', 0),
    '#description' => t("Otherwise cURL will only be used as fallback when drupal_http_request fails."),
  );
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
  $devel_module = l(t('Devel module'), 'https://drupal.org/project/devel', array(
      'attributes' => array(
        'title' => t('Devel | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  // Disable the Devel output until it is available.
  if (!module_exists('devel')) {
    if (variable_get('flickr_debug', 0) == 2) {
      variable_set('flickr_debug', 1);
      drupal_set_message(t("The debug output has been set to 'Flickr response only'. 'Plus Devel' has been disabled until you enable the !devel_module.", array('!devel_module' => $devel_module)), 'warning', FALSE);
    }
  }
  $form['advanced_settings']['flickr_debug'] = array(
    '#type' => 'radios',
    '#title' => t('Enable Debug Output'),
    '#options' => array(
      t('None'),
      t('Flickr response only (as a link to an XML page in a debug message)'),
      t('Plus Devel (Flickr response plus additional output)'),
    ),
    '#description' => t('Display the Flickr XML response, all passed photo/album arguments and HTTP requests/response objects via the !devel_module.', array('!devel_module' => $devel_module)),
    '#default_value' => variable_get('flickr_debug', 0),
  );
  // Disable the Devel output until it is available.
  if (!module_exists('devel')) {
    $form['advanced_settings']['flickr_debug']['#options'] = array(
      t('None'),
      t('Flickr response only (as a link to an XML page in a debug message)'),
      '<span class="grayed-out">' . t('Plus Devel (Flickr response plus additional output') . '</span> <span class="grayed-out-help">' . t('Disabled until you enable the !devel_module.', array('!devel_module' => $devel_module)) . '</span> ',
    );
    $form['advanced_settings']['flickr_debug'][2]['#disabled'] = TRUE;
    $form['advanced_settings']['flickr_debug']['#description'] = t('Display the Flickr XML response.');
  }
  $form['block_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 16,
  );
  if (module_exists('flickr_block')) {
    $form['block_settings']['#description'] = t('Clear the cache on form submit.');
  }
  $date = l(t('Date'), 'https://www.drupal.org/project/date', array(
    'attributes' => array(
      'title' => t('Project page on Drupal.org'),
      'target' => '_blank',
    ),
  ));
  $geofield = l(t('Geofield'), 'https://www.drupal.org/project/geofield', array(
    'attributes' => array(
      'title' => t('Project page on Drupal.org'),
      'target' => '_blank',
    ),
  ));
  $form['block_settings']['flickr_smart'] = array(
    '#type' => 'checkbox',
    '#title' => t("Smart install of Flickr Block"),
    '#default_value' => variable_get('flickr_smart', 1),
    '#description' => t("On install of Flickr Block auto create Flickr taxonomy, date and geo fields on all node types to grab Flickr photos related to the node on the same page as a Flickr block based on tags, a date or a location. Enable Taxonomy (core), !date (including date_popup) and !geofield before enabling Flickr Block for the first time (or uninstall it first).", array('!date' => $date, '!geofield' => $geofield)),
  );
  // If a block variable exists, the Flickr Block module has not been
  // uninstalled, thus disable the setting relevant only during installation.
  global $conf;
  if (module_exists('flickr_block') || isset($conf['flickr_block_photos_per_set'])) {
    $form['block_settings']['flickr_smart']['#disabled'] = TRUE;
    $form['block_settings']['flickr_smart']['#title'] = '<span class="grayed-out">' . t("Smart install of Flickr Block") . '</span> | ' . t('Disabled until uninstall of Flickr Block.');
    $form['block_settings']['flickr_smart']['#description'] = '<span class="grayed-out">' . t("On install of Flickr Block auto create Flickr taxonomy, date and geo fields on all node types to grab Flickr photos related to the node on the same page as a Flickr block based on tags, a date or a location. Enable Taxonomy (core), !date (including date_popup) and !geofield before enabling Flickr Block for the first time (or uninstall it first).", array('!date' => $date, '!geofield' => $geofield)) . '</span>';
  }
  $flickr_block = l(t('Flickr Block'), 'admin/modules', array(
      'attributes' => array(
        'title' => t('Display Flickr photos in blocks.'),
      ),
  ));
  if (!module_exists('flickr_block')) {
    $form['block_settings']['flickr_block'] = array(
      '#markup' => t("Display Flickr photos in blocks by enabling the !flickr_block sub-module.", array('!flickr_style' => $flickr_style)),
    );
  }
  $form['flickr_cc'] = array(
    '#type' => 'checkbox',
    '#title' => t("Flush the cache on form submit to see your changes instantly."),
    '#default_value' => variable_get('flickr_cc', 0),
    '#description' => t("Note that form submit will be slower. Your content will be rebuilt at the first visit. Your choice will be 'remembered' for your next visit to this configuration page."),
    '#weight' => 97,
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
  $limit7 = trim($form_state['values']['flickr_per_page']);
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
  // Validate the number to return on a random or views sorted API requests.
  if (!is_numeric($limit7) || $limit7 < 20 || $limit7 > 500) {
    form_set_error('flickr_per_page', t('Set a number from 20 to 500.'));
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
