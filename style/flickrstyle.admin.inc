<?php
/**
 * @file
 * The admin settings for the Flickr Style module.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function flickrstyle_form_flickr_admin_settings_alter(&$form, &$form_state) {
  $form['#validate'][] = 'flickrstyle_admin_settings_validate';
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
    // We need a shadow and/or border before we can use a style on the caption
    // and a reset to 'No' of it if so.
    if (gettype($styles['shadow']) == 'integer' && gettype($styles['border']) == 'integer') {
      variable_set('flickrstyle_caption', 0);
      drupal_set_message(t("The caption style has been disabled until you set a 'border' and/or 'shadow' in the 'Image styles'. It will be available again after saving the form."), 'warning', FALSE);
    }
    $form['css_settings']['flickrstyle_caption'] = array(
      '#type' => 'radios',
      '#title' => t('Apply the image style also on the caption?'),
      '#options' => array(
        t('No'),
        t('Yes, separate from the image'),
        t('Yes, wrapped around the image and caption ("Polaroid" effect if applied with a border)'),
      ),
      // Defaults to 'No'
      '#default_value' => variable_get('flickrstyle_caption', 1),
      '#weight' => 10,
    );
    // We need a shadow and/or border before we can use a style on the caption.
    if (gettype($styles['shadow']) == 'integer' && gettype($styles['border']) == 'integer') {
      $form['css_settings']['flickrstyle_caption']['#disabled'] = TRUE;
      $form['css_settings']['flickrstyle_caption']['#options'] = array(
        '<span class="grayed-out">' . t('No') . '</span>',
        '<span class="grayed-out">' . t('Yes, separate from the image') . '</span>',
        '<span class="grayed-out">' . t('Yes, wrapped around the image and caption') . '</span>',
      );
      $form['css_settings']['flickrstyle_caption']['#description'] = t('Disabled until a shadow and/or border image style is selected. Select above and save the form to make a caption style available.');
    }
    $form['css_settings']['flickrstyle_nostyle'] = array(
      '#type' => 'checkbox',
      '#title' => t("Disable the above styles on a caption with only the text 'Flickr' (simple link)"),
      '#default_value' => variable_get('flickrstyle_nostyle', 1),
      '#weight' => 11,
    );
    // Disable the 'Enlarge' style until a border is set or shadow unselected.
    if (gettype($styles['shadow']) != 'integer' && gettype($styles['border']) == 'integer') {
      if (variable_get('flickrstyle_emphasize', 2) == 2) {
        variable_set('flickrstyle_emphasize', 3);
        drupal_set_message(t("The emphasize style has been set to 'Zoom'. 'Enlarge' has been disabled until you set a 'border' or unselect 'shadow' in the 'Image styles'. It will be available again after saving the form."), 'warning', FALSE);
      }
    }
    $form['css_settings']['flickrstyle_emphasize'] = array(
      '#type' => 'radios',
      '#title' => t('Emphasize the image on hover?'),
      '#options' => array(
        t('No'),
        t('Enlarge slightly (includes the caption)'),
        t('Enlarge (image only) (*)'),
        t('Zoom (*)'),
        t('Magnifier icon'),
      ),
      // Defaults to 'Enlarge slightly'.
      '#default_value' => variable_get('flickrstyle_emphasize', 2),
      '#description' => t("(*): The 'Zoom' and 'Enlarge' styles use bigger images to maintain the resolution. This has a slight performance impact and the size 'big square' (q) is not square but 150px on the longest side to avoid distortion on older browsers."),
      '#weight' => 12,
    );
    // Disable the 'Enlarge' style until a border is set or shadow unselected.
    if (gettype($styles['shadow']) != 'integer' && gettype($styles['border']) == 'integer') {
      $form['css_settings']['flickrstyle_emphasize']['#options'] = array(
        t('No'),
        t('Enlarge slightly (includes the caption)'),
        '<span class="grayed-out">' . t('Enlarge (image only) (*)') . '</span> <span class="grayed-out-help">' . t("Disabled until you set a 'border' or unselect 'shadow' in the 'Image styles' above. It will be available after saving the form.") . '</span> ',
        t('Zoom (*)'),
        t('Magnifier icon'),
      );
      $form['css_settings']['flickrstyle_emphasize'][2]['#disabled'] = TRUE;
    }
  }
  else {
    $form['css_settings']['flickrstyle_suppressed'] = array(
      '#markup' => t("Styling options are not displayed until the module's CSS is enabled again (the checkbox above)."),
    );
  }
}

/**
 * Validate user input.
 */
function flickrstyle_admin_settings_validate($form, &$form_state) {
  // Put validation rules here.
}
