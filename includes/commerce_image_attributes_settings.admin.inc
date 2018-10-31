<?php

/**
 * Implements hook_action_info_alter().
 */
function hdmd_commerce_image_attributes_settings_form($form, &$form_state) {
  $form['settings'] = array(
    '#type' => 'container',
    '#id' => 'hdmd-image-settings'
  );
  $form['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Product type'),
    '#options' => _get_product_types_list(),
    '#default_value' => variable_get('hdmd_commerce_image_attributes_type'),
    '#ajax' => array(
      'wrapper' => 'block-system-main > div > form',
      'callback' => 'hdmd_commerce_image_attributes_settings_callback',
    ),
  );
  
  $type = variable_get('hdmd_commerce_image_attributes_type');
  if (isset($form_state['values']['type']) && !empty($form_state['values']['type'])) {
    $type = $form_state['values']['type'];
  }
  
  $form['settings']['field'] = array(
    '#type' => 'select',
    '#title' => t('Image Field'),
    '#options' => _get_product_type_field($type),
    '#default_value' => variable_get('hdmd_commerce_image_attributes_field'),
    '#empty_option' => t('- Select -'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration')
  );
  
  return $form;
}

function hdmd_commerce_image_attributes_settings_form_submit($form, &$form_state) {
  variable_set('hdmd_commerce_image_attributes_type', $form_state['values']['type']);
  variable_set('hdmd_commerce_image_attributes_field', $form_state['values']['field']);
  
  drupal_set_message(t('The configuration options have been saved.'));
}

function hdmd_commerce_image_attributes_settings_callback(&$form, &$form_state) {
  return $form;
}

function _get_product_types_list() {
  $query = db_select('commerce_product_type', 't')
          ->fields('t', array('type', 'name'))
          ->orderBy('name', 'DESC')
          ->range(0, 5);
  $types = $query->execute()->fetchAll();
  
  foreach ($types as $type) {
    $options[$type->type] = $type->name;
  }
  
  return isset($options) ? $options : array();
}

function _get_product_type_field($type = NULL) {
  $fields = field_info_instances('commerce_product', $type);
  
  foreach ($fields as $field) {
    if (isset($field['widget']['module']) && $field['widget']['module'] != 'image') {
      continue;
    }
    if (isset($field['field_name'])) {
      $options[$field['field_name']] = t($field['label']);
    }
  }
  
  return isset($options) ? $options : array();
}
