<?php

/**
 * @file
 * Manages webform external administration UI
 */

/**
 * Callback function to edit sitewide configuration and defaults
 */
function webform_external_config($form, &$form_state) {

	$form = array();

	$form['civi'] = array(
		'#type' => 'fieldset',
		'#title' => 'CiviCRM Integration',
		'#collapsible' => TRUE,
		'#collapsed' => !variable_get('webform_external_civi_enabled'),
	);
	$form['civi']['webform_external_civi_enabled'] = array(
    	'#type' => 'checkbox',
		'#title' => t('CiviCRM enabled'),
		'#default_value' => variable_get('webform_external_civi_enabled'),
	);
	$form['civi']['webform_external_civi_rest_url'] = array(
		'#type' => 'textfield',
		'#title' => t('CiviCRM REST URL'),
		'#default_value' => variable_get('webform_external_civi_rest_url'),
	);
	$form['civi']['webform_external_civi_key'] = array(
		'#type' => 'textfield',
		'#title' => t('CiviCRM key'),
		'#default_value' => variable_get('webform_external_civi_key'),
	);
	$form['civi']['webform_external_civi_api_key'] = array(
		'#type' => 'textfield',
		'#title' => t('CiviCRM API key'),
		'#default_value' => variable_get('webform_external_civi_api_key'),
	);

	$form['salsa'] = array(
		'#type' => 'fieldset',
		'#title' => 'Salsa Integration',
		'#collapsible' => TRUE,
		'#collapsed' => !variable_get('webform_external_salsa_enabled'),
	);
	$form['salsa']['webform_external_salsa_enabled'] = array(
    	'#type' => 'checkbox',
		'#title' => t('Salsa enabled'),
		'#default_value' => variable_get('webform_external_salsa_enabled'),
	);
	$form['salsa']['webform_external_salsa_node_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa node URL'),
		'#default_value' => variable_get('webform_external_salsa_node_url'),
	);
	$form['salsa']['webform_external_salsa_organization_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa organization key'),
		'#default_value' => variable_get('webform_external_salsa_organization_key'),
	);
	$form['salsa']['webform_external_salsa_chapter_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa chapter key'),
		'#default_value' => variable_get('webform_external_salsa_chapter_key'),
	);
	$form['salsa']['webform_external_salsa_account_email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email of Salsa user account'),
		'#default_value' => variable_get('webform_external_salsa_account_email'),
	);
	$form['salsa']['webform_external_salsa_password'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa account password'),
		'#default_value' => variable_get('webform_external_salsa_password'),
	);
	$form['salsa']['webform_external_salsa_group_ids'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa group ids'),
		'#default_value' => variable_get('webform_external_salsa_group_ids'),
	);
	$form['salsa']['webform_external_salsa_action_keys'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa action keys'),
		'#default_value' => variable_get('webform_external_salsa_action_keys'),
	);
	$form['salsa']['webform_external_salsa_autoresponder_ids'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa autoresponder ids'),
		'#default_value' => variable_get('webform_external_salsa_autoresponder_ids'),
	);
	$form['salsa']['webform_external_salsa_tags'] = array(
		'#type' => 'textfield',
		'#title' => t('Salsa tag'),
		'#default_value' => variable_get('webform_external_salsa_tags'),
	);

	return system_settings_form($form);
}

function webform_external_config_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['webform_external_civi_rest_url'] !== '' && !valid_url($values['webform_external_civi_rest_url'],true)) {
    form_set_error('webform_external_civi_rest_url', t('CiviCRM REST URL must be a valid URL.'));
  }
  if ($values['webform_external_salsa_node_url'] !== '' && !valid_url($values['webform_external_salsa_node_url'],true)) {
    form_set_error('webform_external_salsa_node_url', t('Salsa node URL must be a valid URL.'));
  }
  if (!_webform_external_validate_integer($values['webform_external_salsa_organization_key'])) {
    form_set_error('webform_external_salsa_organization_key', t('Salsa Organization Key must be an integer.'));
  }
  if (!_webform_external_validate_integer($values['webform_external_salsa_chapter_key'])) {
    form_set_error('webform_external_salsa_chapter_key', t('Salsa Chapter Key must be an integer.'));
  }
  if ($values['webform_external_salsa_account_email'] !== '' && !valid_email_address($values['webform_external_salsa_account_email'])) {
    form_set_error('webform_external_salsa_account_email', t('Salsa Account Email must be a valid email.'));
  }
  if (!_webform_external_validate_multiple_ids($values['webform_external_salsa_group_ids'])) {
    form_set_error('webform_external_salsa_group_ids', t('Salsa Group ids must be comma-separated integers.'));
  }
  if (!_webform_external_validate_multiple_ids($values['webform_external_salsa_action_keys'])) {
    form_set_error('webform_external_salsa_action_keys', t('Salsa Action Keys must be comma-separated integers.'));
  }
  if (!_webform_external_validate_multiple_ids($values['webform_external_salsa_autoresponder_ids'])) {
    form_set_error('webform_external_salsa_autoresponder_ids', t('Salsa Autoresponder ids must be comma-separated integers.'));
  }
}

/**
 * Callback function to edit options on a single webform
 */
function webform_external_manage_options($form, &$form_state, $node) {

  // get current settings
  $current = new stdClass();
  $query = db_select('webform_external_options','o');
  $query->fields('o')->condition('nid',$node->nid);
  $result = $query->execute();
  foreach ($result as $field) {
	$current = $field;
  }

  $form = array();

  if (variable_get('webform_external_civi_enabled',false)) {
  $form['civi'] = array(
    '#type' => 'fieldset',
    '#title' => 'CiviCRM Integration',
    '#collapsible' => TRUE,
    '#collapsed' => isset($current->civi_enabled) ? !$current->civi_enabled : true,
  );
  $form['civi']['civi_enabled'] = array (
    '#type' => 'checkbox',
    '#title' => 'Enable CiviCRM integration',
	'#default_value' => isset($current->civi_enabled) ? $current->civi_enabled : false,
  );
  $form['civi']['civi_rest_url'] = array(
    '#type' => 'textfield',
    '#title' => 'URL of CiviCRM REST file',
	'#default_value' => (isset($current->civi_rest_url) && $current->civi_rest_url) ? $current->civi_rest_url : variable_get('webform_external_civi_rest_url',''),
  );
  $form['civi']['civi_key'] = array(
    '#type' => 'textfield',
    '#title' => 'CiviCRM key',
	'#default_value' => (isset($current->civi_hey) && $current->civi_key) ? $current->civi_key : variable_get('webform_external_civi_key',''),
  );
  $form['civi']['civi_api_key'] = array(
    '#type' => 'textfield',
    '#title' => 'CiviCRM API key',
	'#default_value' => (isset($current->civi_api_key) && $current->civi_api_key) ? $current->civi_api_key : variable_get('webform_external_civi_api_key',''),
  );
  $form['civi']['civi_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'CiviCRM Field Mapping',
    '#description' => 'One field per line. Use format civi_field|form_key.  If CiviCRM field matches the form_key, you can just enter the key once, without the | character.',
	'#default_value' => isset($current->civi_field_mapping) ? $current->civi_field_mapping : '',
  );
  $form['civi']['civi_tag_ids'] = array(
    '#type' => 'textfield',
    '#title' => 'CiviCRM Tag IDs',
	'#default_value' => isset($current->civi_tag_ids) ? $current->civi_tag_ids : '',
  );
  } // end if civi_enabled

  if (variable_get('webform_external_salsa_enabled',false)) {
  $form['salsa'] = array(
    '#type' => 'fieldset',
    '#title' => 'SALSA Integration',
    '#collapsible' => TRUE,
    '#collapsed' => isset($current->salsa_enabled) ? !$current->salsa_enabled : true,
  );
  $form['salsa']['salsa_enabled'] = array (
    '#type' => 'checkbox',
    '#title' => 'Enable Salsa integration',
	'#default_value' => isset($current->salsa_enabled) ? $current->salsa_enabled : false,
  );
  $form['salsa']['salsa_node_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa node URL (required)',
	'#default_value' => $current->salsa_node_url ? $current->salsa_node_url : variable_get('webform_external_salsa_node_url',''),
  );
  $form['salsa']['salsa_organization_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa organization key (required)',
	'#default_value' => $current->salsa_organization_key ? $current->salsa_organization_key : variable_get('webform_external_salsa_organization_key',''),
  );
  $form['salsa']['salsa_chapter_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa chapter key (optional)',
	'#default_value' => $current->salsa_chapter_key ? $current->salsa_chapter_key : variable_get('webform_external_salsa_chapter_key',''),
  );
  $form['salsa']['salsa_account_email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email of Salsa user account (required)',
	'#default_value' => $current->salsa_account_email ? $current->salsa_account_email : variable_get('webform_external_salsa_account_email',''),
  );
  $form['salsa']['salsa_password'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa account password (required)',
	'#default_value' => $current->salsa_password ? $current->salsa_password : variable_get('webform_external_salsa_password',''),
  );
  $form['salsa']['salsa_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Field Mapping (required)',
    '#description' => 'One field per line. Use format Salsa_Field|form_key.  If salsa field matches the form_key, you can just enter the key once, without the | character.',
	'#default_value' => $current->salsa_field_mapping,
  );
  $form['salsa']['salsa_boolean_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Checkbox to Boolean Field Mapping',
    '#description' => 'One form_key per line. If the value of the key from multiple checkboxes matches a Salsa Field key, that Salsa Field will be set to 1.',
	'#default_value' => $current->salsa_boolean_field_mapping,
  );
  $form['salsa']['salsa_groups_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Checkbox to Group Mapping',
    '#description' => 'One form_key per line. If the value of the key from multiple checkboxes matches a Salsa Group key, the contact will be added to that group.',
	'#default_value' => $current->salsa_groups_field_mapping,
  );
  $form['salsa']['salsa_group_ids'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Group Ids (optional)',
    '#description' => 'If the id (key) of one or more Salsa groups is entered here, the contact will be added to that group. Must be one or more integers, separated by commas.',
	'#default_value' => $current->salsa_group_ids ? $current->salsa_group_ids : variable_get('webform_external_salsa_group_ids',''),
  );
  $form['salsa']['salsa_action_keys'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Action Keys (optional)',
    '#description' => 'If the id (key) of one or more Salsa actions is entered here, the contact will be marked as having participated in that action. Must be one or more integers, separated by commas.',
	'#default_value' => $current->salsa_action_keys ? $current->salsa_action_keys : variable_get('webform_external_salsa_action_keys',''),
  );
  $form['salsa']['salsa_autoresponder_ids'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Autoresponder Ids (optional)',
    '#description' => 'If the (key) of one or more Salsa autoresponders is entered here, the contact will be sent that autoresponder. Must be one or more integers, separated by commas.',
	'#default_value' => $current->salsa_autoresponder_ids ? $current->salsa_autoresponder_ids : variable_get('webform_external_salsa_autoresponder_ids',''),
  );
  $form['salsa']['salsa_tags'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa Tags (optional)',
    '#description' => 'If one or more tags are entered here, the contact will be tagged in Salsa.  Separate multiple tags with commas.',
	'#default_value' => $current->salsa_tags ? $current->salsa_tags : variable_get('webform_external_salsa_tags',''),
  );
  } // end if salsa_enabled

  if (count($form)) {

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );
  $form['oid'] = array(
    '#type' => 'hidden',
	'#value' => isset($current->oid) ? $current->oid : '',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save Options',
  );

  } else {

  $form['markup'] = array(
	'#type' => 'markup',
	'#markup' => '<p>'.t('No External APIs have been enabled.').' '.l(t('Enable one or more external APIs.'),'admin/config/content/webform-external').'</p>',
  );

  }

  return $form;

}

function webform_external_manage_options_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (variable_get('webform_external_civi_enabled',false)) {

  if ($values['civi_rest_url'] !== '' && !valid_url($values['civi_rest_url'],true)) {
    form_set_error('civi_rest_url', t('CiviCRM REST URL must be a valid URL.'));
  }
  if (!_webform_external_validate_multiple_ids($values['civi_tag_ids'])) {
    form_set_error('civi_tag_ids', t('CiviCRM Tag ids must be comma-separated integers.'));
  }

  } // end if civi_enabled

  if (variable_get('webform_external_salsa_enabled',false)) {

  if ($values['salsa_node_url'] !== '' && !valid_url($values['salsa_node_url'],true)) {
    form_set_error('salsa_node_url', t('Salsa node URL must be a valid URL.'));
  }
  if (!_webform_external_validate_integer($values['salsa_organization_key'])) {
    form_set_error('salsa_organization_key', t('Salsa Organization Key must be an integer.'));
  }
  if (!_webform_external_validate_integer($values['salsa_chapter_key'])) {
    form_set_error('salsa_chapter_key', t('Salsa Chapter Key must be an integer.'));
  }
  if ($values['salsa_account_email'] !== '' && !valid_email_address($values['salsa_account_email'])) {
    form_set_error('salsa_account_email', t('Salsa Account Email must be a valid email.'));
  }
  if (!_webform_external_validate_multiple_ids($values['salsa_group_ids'])) {
    form_set_error('salsa_group_ids', t('Salsa Group ids must be comma-separated integers.'));
  }
  if (!_webform_external_validate_multiple_ids($values['salsa_action_keys'])) {
    form_set_error('salsa_action_keys', t('Salsa Action Keys must be comma-separated integers.'));
  }
  if (!_webform_external_validate_multiple_ids($values['salsa_autoresponder_ids'])) {
    form_set_error('salsa_autoresponder_ids', t('Salsa Autoresponder ids must be comma-separated integers.'));
  }

  } // end if Salsa enabled
}

function webform_external_manage_options_submit($form, &$form_state) {
  $values_to_set = array('nid',
	'civi_enabled','civi_rest_url','civi_key','civi_api_key','civi_field_mapping','civi_tag_ids',
	'salsa_enabled','salsa_node_url','salsa_organization_key','salsa_chapter_key',
	'salsa_account_email','salsa_password','salsa_field_mapping',
	'salsa_boolean_field_mapping','salsa_groups_field_mapping',
	'salsa_group_ids','salsa_action_keys','salsa_autoresponder_ids','salsa_tags');
  $values = $form_state['values'];
  echo '</pre>';
  $record = new stdClass();
  foreach($values_to_set as $k) {
    if (isset($values[$k])) { $record->$k = $values[$k]; }
  }
  /* mail('jk@webskillet.com','$values and $record',print_r($values,1)."\n\n".print_r($record,1),"From: Webskillet Dev <dev@webskillet.com>\r\n"); */
  if ($values['oid']) {
    $record->oid = $values['oid'];
    drupal_write_record('webform_external_options', $record, 'oid');
  } else {
    drupal_write_record('webform_external_options', $record);
  }
  drupal_set_message(t('Configuration options have been saved'));
}

/**
 * Utility functions
 */

function _webform_external_validate_integer($id) {
	if ($id !== '' && (!is_numeric($id) || intval($id) != $id)) { return false; }
	return true;
}

function _webform_external_validate_multiple_ids($ids) {
	$id_array = explode(',',$ids);
	foreach ($id_array as $id) {
		$id = trim($id);
		if (!_webform_external_validate_integer($id)) { return false; }
	}
	return true;
}
