<?php
/**
 * @file
 * The admin settings for the Flickr Style module
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function flickrstyle_form_flickr_admin_settings_alter(&$form, &$form_state) {
  // Do not display CSS styling options if the module's CSS is disabled.
  if (variable_get('flickr_css', 1)) {
    $options = array(
      'rounded' => t('Rounded corners'),
      'shadow' => t('Shadow'),
      'border' => t('Border'),
    );
    $form['css_settings']['flickrstyle_styles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Image styles'),
      '#default_value' => variable_get('flickrstyle_styles', array(
        'rounded' => 'rounded',
        'shadow' => 'shadow',
        'border' => 'border',
      )),
      '#options' => $options,
      '#weight' => 8,
    );
    $styles = variable_get('flickrstyle_styles', array(
      'rounded' => 'rounded',
      'shadow' => 'shadow',
      'border' => 'border',
    ));
    switch (variable_get('flickrstyle_caption', 0)) {
      case 0:
        // Reset of the caption width if not altered by the user.
        if (variable_get('flickr_caption_padding', 0) == 10 || variable_get('flickr_caption_padding', 0) == 16) {
          variable_set('flickr_caption_padding', 0);
        }
        break;

      case 1:
        // Adjustment of the caption width if not altered already.
        if (variable_get('flickr_caption_padding', 0) == 0 || variable_get('flickr_caption_padding', 0) == 10) {
          variable_set('flickr_caption_padding', 16);
        }
        break;

      case 2:
        // Adjustment of the caption width if not altered already.
        if (variable_get('flickr_caption_padding', 0) == 0 || variable_get('flickr_caption_padding', 0) == 16) {
          variable_set('flickr_caption_padding', 10);
        }
        break;
    }
    // We need a shadow and/or border before we can use a style on the caption
    // and a reset to 'No' of it if so.
    if (gettype($styles['shadow']) == 'integer' && gettype($styles['border']) == 'integer') {
      variable_set('flickrstyle_caption', 0);
    }
    $form['css_settings']['flickrstyle_caption'] = array(
      '#type' => 'radios',
      '#title' => t('Apply the image style also on the caption?'),
      '#options' => array(
        t('No'),
        t('Yes, separate from the image'),
        t('Yes, wrapped around the image and caption'),
      ),
      // Defaults to 'No'
      '#default_value' => variable_get('flickrstyle_caption', 0),
      '#weight' => 9,
    );
    // We need a shadow and/or border before we can use a style on the caption.
    if (gettype($styles['shadow']) == 'integer' && gettype($styles['border']) == 'integer') {
      $form['css_settings']['flickrstyle_caption']['#disabled'] = TRUE;
      $form['css_settings']['flickrstyle_caption']['#description'] = t('Disabled until a shadow and/or border image style is selected. Select above and save the form to make a caption style available.');
    }
    $form['css_settings']['flickrstyle_emphasize'] = array(
      '#type' => 'radios',
      '#title' => t('Emphasize the image on hover?'),
      '#options' => array(
        t('No'),
        t('Enlarge slightly'),
        t('Zoom in'),
        t('Magnifier icon'),
      ),
      // Defaults to 'Enlarge slightly'.
      '#default_value' => variable_get('flickrstyle_emphasize', 1),
      '#weight' => 10,
    );
  }
  else {
    $form['css_settings']['flickrstyle_suppressed'] = array(
      '#markup' => t("Styling options are not displayed until the module's CSS is enabled again (the checkbox above)."),
    );
  }
}
