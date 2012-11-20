<?php

/**
 * Menu callback for the settings page.
 * @return mixed
 */
function support_bar_admin_settings() {
  $form = array();

  $form['support_bar_enabled'] = array(
    '#title' => t('Enable Support Bar Display'),
    '#description' => t('Enable the display of the support bar. If disabled, it will not load.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('support_bar_enabled', 1),
  );

  $form['support_bar_phone_number'] = array(
    '#title' => t('Company Phone Number'),
    '#description' => t('Enter the phone number to the development/maintenance company that handles this website.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('support_bar_phone_number', ''),
  );

  $form['support_bar_email_address'] = array(
    '#title' => t('Company Email Address'),
    '#description' => t('Enter the email address where all support queries should be sent.'),
    '#type' => 'email',
    '#required' => TRUE,
    '#default_value' => variable_get('support_bar_email_address', ''),
  );

  $form['support_bar_url'] = array(
    '#title' => t('Company URL'),
    '#description' => t('Enter the URL of the development/maintenance company the logo should link to.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('support_bar_url', 'http://www.example.com'),
  );

  $form['support_bar_url'] = array(
    '#title' => t('Company URL'),
    '#description' => t('Enter the URL the development/maintenance company the logo should link to.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('support_bar_url', 'http://www.example.com'),
  );

  $form['support_bar_image_fid'] = array(
    '#title' => t('Company Logo'),
    '#description' => t('Upload the image to be used as the company logo on the support bar. Small transparent png files work best, with an image resolution of under 150x80. Allowed extensions: png jpg jpeg.'),
    '#type' => 'managed_file',
    '#default_value' => variable_get('support_bar_image_fid', ''),
    '#upload_location' => 'public://support-bar',
    '#upload_validators' => array(
      'file_validate_extensions' => array('png jpg jpeg'),
    ),
  );

  $form['#validate'][] = 'support_bar_validate';
  $form['#submit'][] = 'support_bar_submit';

  return system_settings_form($form);
}

/**
 * Form validation.
 * @param $form
 * @param $form_state
 */
function support_bar_validate($form, $form_state) {
  if ($form_state['values']['support_bar_image_fid'] != 0) {
    $file = file_load($form_state['values']['support_bar_image_fid']);
    $validate_size = file_validate_size($file, 102400, 0);
    $validate_dimension = file_validate_image_resolution($file, '150x100', 0);

    if (count($validate_size)) {
      form_set_error('support_bar_image_fid', 'The file you are uploading is too large. Please upload an image smaller than 100kb.');
    }

    if (count($validate_dimension)) {
      form_set_error('support_bar_image_fid', 'The file you are uploading is larger than 200x200. Please upload a smaller image.');
    }
  }
}

/**
 * Form submit handler.
 * @param $form
 * @param $form_state
 */
function support_bar_submit($form, $form_state) {
  if (is_numeric($form_state['values']['support_bar_image_fid']) && $form_state['values']['support_bar_image_fid'] != 0) {
    global $user;
    $file = file_load($form_state['values']['support_bar_image_fid']);

    if ($file->status != FILE_STATUS_PERMANENT) {
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'support_bar', 'image', $user->uid);
      variable_set('support_bar_image_fid', $file->fid);
      variable_set('support_bar_filepath', file_create_url($file->uri));
    }
  } else {
    variable_del('support_bar_filepath');
    variable_del('support_bar_image_fid');
  }
}