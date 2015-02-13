<?php
/**
 * @file
 * The admin settings for the Flickr Filter module.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function flickr_filter_form_flickr_admin_settings_alter(&$form, &$form_state) {
  $form['#validate'][] = 'flickr_filter_admin_settings_validate';
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

/**
 * Validate user input.
 */
function flickr_filter_admin_settings_validate($form, &$form_state) {
  // Put validation rules here.
}