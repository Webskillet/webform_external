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

	$form['actionnetwork'] = array(
		'#type' => 'fieldset',
		'#title' => 'Action Network Integration',
		'#collapsible' => TRUE,
		'#collapsed' => !variable_get('webform_external_actionnetwork_enabled'),
	);
	$form['actionnetwork']['webform_external_actionnetwork_enabled'] = array(
    	'#type' => 'checkbox',
		'#title' => t('Action Network enabled'),
		'#default_value' => variable_get('webform_external_actionnetwork_enabled'),
	);
	$form['actionnetwork']['webform_external_actionnetwork_api_key'] = array(
		'#type' => 'textfield',
		'#title' => t('Action Network API key'),
		'#default_value' => variable_get('webform_external_actionnetwork_api_key'),
	);

	$form['mcommons'] = array(
		'#type' => 'fieldset',
		'#title' => 'Mobile Commons Integration',
		'#collapsible' => TRUE,
		'#collapsed' => !variable_get('webform_external_mcommons_enabled'),
	);
	$form['mcommons']['webform_external_mcommons_enabled'] = array(
    	'#type' => 'checkbox',
		'#title' => t('Mobile Commons enabled'),
		'#default_value' => variable_get('webform_external_mcommons_enabled'),
	);
	$form['mcommons']['webform_external_mcommons_username'] = array(
		'#type' => 'textfield',
		'#title' => t('Mobile Commons Username'),
		'#default_value' => variable_get('webform_external_mcommons_username'),
	);
	$form['mcommons']['webform_external_mcommons_password'] = array(
		'#type' => 'textfield',
		'#title' => t('Mobile Commons Password'),
		'#default_value' => variable_get('webform_external_mcommons_password'),
	);
	$form['mcommons']['webform_external_mcommons_subdomain'] = array(
		'#type' => 'textfield',
		'#title' => t('Mobile Commons Subdomain'),
		'#default_value' => variable_get('webform_external_mcommons_subdomain','https://secure.mcommons.com'),
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
  if ($values['webform_external_mcommons_subdomain'] !== '' && !valid_url($values['webform_external_mcommons_subdomain'],true)) {
    form_set_error('webform_external_mcommons_subdomain', t('Mobile Commons Subdomain must be a valid URL.'));
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
    '#description' => t('<p>One field per line. Use format civi_field|form_key.  If CiviCRM field matches the form_key, you can just enter the key once, without the | character.</p><p>Use <strong>groups</strong> or <strong>tags</strong> to add group or tag ids (value must be an integer, or multiple integers)</p><p>If you use <strong>boolean</strong>, followed by one or more comma-separated form_keys, then if the value of that form_key\'s component evaluates to one or more CiviCRM field names, those CiviCRM fields will be set to 1/true.</p>'),
	'#default_value' => isset($current->civi_field_mapping) ? $current->civi_field_mapping : '',
  );
  $form['civi']['civi_field_defaults'] = array(
    '#type' => 'textarea',
    '#title' => 'CiviCRM Default Mapping',
    '#description' => t('One field per line. Use format civi_field|value. Use <strong>groups</strong> or <strong>tags</strong> to add one or more group or tag id to the record (multiple values should be comma-separated)'),
	'#default_value' => isset($current->civi_field_defaults) ? $current->civi_field_defaults : '',
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
	'#default_value' => isset($current->salsa_node_url) ? $current->salsa_node_url : variable_get('webform_external_salsa_node_url',''),
  );
  $form['salsa']['salsa_organization_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa organization key (required)',
	'#default_value' => isset($current->salsa_organization_key) ? $current->salsa_organization_key : variable_get('webform_external_salsa_organization_key',''),
  );
  $form['salsa']['salsa_chapter_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa chapter key (optional)',
	'#default_value' => isset($current->salsa_chapter_key) ? $current->salsa_chapter_key : variable_get('webform_external_salsa_chapter_key',''),
  );
  $form['salsa']['salsa_account_email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email of Salsa user account (required)',
	'#default_value' => isset($current->salsa_account_email) ? $current->salsa_account_email : variable_get('webform_external_salsa_account_email',''),
  );
  $form['salsa']['salsa_password'] = array(
    '#type' => 'textfield',
    '#title' => 'Salsa account password (required)',
	'#default_value' => isset($current->salsa_password) ? $current->salsa_password : variable_get('webform_external_salsa_password',''),
  );
  $form['salsa']['salsa_field_mapping'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Field Mapping (required)',
    '#description' => '<p>One field per line. Use format Salsa_Field|form_key.  If salsa field matches the form_key, you can just enter the key once, without the | character.</p><p>Use <strong>groups</strong>, <strong>tags</strong>, <strong>action_keys</strong> and <strong>autoresponder_ids</strong> to add group keys, tags, action_keys and autoresponder_ids (value must be an integer, or multiple comma-separated integers).</p><p>If you use <strong>boolean</strong, followed by one or more comma-separated form_keys, then if the value of that form_key\'s component evaluates to one or more Salsa field names, those Salsa fields will be set to 1/true.</p>',
	'#default_value' => isset($current->salsa_field_mapping) ? $current->salsa_field_mapping : '',
  );
  $form['salsa']['salsa_field_defaults'] = array(
    '#type' => 'textarea',
    '#title' => 'Salsa Default Mapping (required)',
    '#description' => 'One field per line. Use format Salsa_Field|value. Use <strong>groups</strong>, <strong>tags</strong>, <strong>action_keys</strong> and <strong>autoresponder_ids</strong> as above',
	'#default_value' => isset($current->salsa_field_defaults) ? $current->salsa_field_defaults : '',
  );

  } // end if salsa_enabled

	if (variable_get('webform_external_actionnetwork_enabled',false)) {

		$form['actionnetwork'] = array(
			'#type' => 'fieldset',
			'#title' => 'ActionNetwork Integration',
			'#collapsible' => TRUE,
			'#collapsed' => isset($current->actionnetwork_enabled) ? !$current->actionnetwork_enabled : true,
		);
		$form['actionnetwork']['actionnetwork_enabled'] = array (
			'#type' => 'checkbox',
			'#title' => 'Enable Action Network integration',
			'#default_value' => isset($current->actionnetwork_enabled) ? $current->actionnetwork_enabled : false,
		);
		$form['actionnetwork']['actionnetwork_api_key'] = array(
			'#type' => 'textfield',
			'#title' => 'Action Network API Key (required)',
			'#description' => 'API Key associated with Action Network group',
			'#default_value' => isset($current->actionnetwork_api_key) ? $current->actionnetwork_api_key : variable_get('webform_external_actionnetwork_api_key',''),
		);
		$form['actionnetwork']['actionnetwork_field_mapping'] = array(
			'#type' => 'textarea',
			'#title' => 'Action Network Field Mapping',
			'#description' => t('<p>One field per line. Use format actionnetwork_field|form_key.  If Action Network field matches the form_key, you can just enter the key once, without the | character.</p><p>Use <strong>tags</strong> to add one or more tags (multiple values may be separated by commas, and tags must already exist in Action Network)</p><p>If you use <strong>boolean</strong>, followed by one or more comma-separated form_keys, then if the value of that form_key\'s component evaluates to one or more Action Network field names, those Action Network fields will be set to 1/true.</p>'),
			'#default_value' => isset($current->actionnetwork_field_mapping) ? $current->actionnetwork_field_mapping : '',
		);
		$form['actionnetwork']['actionnetwork_field_defaults'] = array(
			'#type' => 'textarea',
			'#title' => 'Action Network Field Defaults',
			'#description' => t('One field per line. Use format actionnetwork_field|value. Use <strong>tags</strong> to add one or more tag to the record (multiple values should be comma-separated, and tags must already exist in Action Network)'),
			'#default_value' => isset($current->actionnetwork_field_defaults) ? $current->actionnetwork_field_defaults : '',
		);

	}

	if (variable_get('webform_external_mcommons_enabled',false)) {

		$form['mcommons'] = array(
			'#type' => 'fieldset',
			'#title' => 'Mobile Commons Integration',
			'#collapsible' => TRUE,
			'#collapsed' => isset($current->mcommons_enabled) ? !$current->mcommons_enabled : true,
		);
		$form['mcommons']['mcommons_enabled'] = array (
			'#type' => 'checkbox',
			'#title' => 'Enable Mobile Commons integration',
			'#default_value' => isset($current->mcommons_enabled) ? $current->mcommons_enabled : false,
		);
		$form['mcommons']['mcommons_username'] = array(
			'#type' => 'textfield',
			'#title' => 'Mobile Commons Username (required)',
			'#default_value' => isset($current->mcommons_username) ? $current->mcommons_username : variable_get('webform_external_mcommons_username',''),
		);
		$form['mcommons']['mcommons_password'] = array(
			'#type' => 'textfield',
			'#title' => 'Mobile Commons Password (required)',
			'#default_value' => isset($current->mcommons_password) ? $current->mcommons_password : variable_get('webform_external_mcommons_password',''),
		);
		$form['mcommons']['mcommons_api_key'] = array(
			'#type' => 'textfield',
			'#title' => 'Mobile Commons Subdomain (required)',
			'#default_value' => isset($current->mcommons_api_key) ? $current->mcommons_api_key : variable_get('webform_external_mcommons_subdomain',''),
		);
		$form['mcommons']['mcommons_field_mapping'] = array(
			'#type' => 'textarea',
			'#title' => 'Mobile Commons Field Mapping',
			'#description' => t('<p>One field per line. Use format mcommons_field|form_key.</p><p>If you use <strong>boolean</strong>, followed by one or more comma-separated form_keys, then if the value of that form_key\'s component evaluates to one or more Action Network field names, those Action Network fields will be set to 1/true.</p>'),
			'#default_value' => isset($current->mcommons_field_mapping) ? $current->mcommons_field_mapping : '',
		);
		$form['mcommons']['mcommons_field_defaults'] = array(
			'#type' => 'textarea',
			'#title' => 'Mobile Commons Field Defaults',
			'#description' => t('One field per line. Use format mcommons_field|value.'),
			'#default_value' => isset($current->mcommons_field_defaults) ? $current->mcommons_field_defaults : '',
		);

	}

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

  } // end if Salsa enabled
}

function webform_external_manage_options_submit($form, &$form_state) {
  $values_to_set = array('nid',
	'civi_enabled','civi_rest_url','civi_key','civi_api_key','civi_field_mapping','civi_field_defaults',
	'salsa_enabled','salsa_node_url','salsa_organization_key','salsa_chapter_key','salsa_account_email','salsa_password','salsa_field_mapping','salsa__field_defaults',
	'actionnetwork_enabled','actionnetwork_api_key','actionnetwork_field_mapping','actionnetwork_field_defaults',
	'mcommons_enabled','mcommons_username','mcommons_password','mcommons_api_key','mcommons_field_mapping','mcommons_field_defaults',
  );
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
