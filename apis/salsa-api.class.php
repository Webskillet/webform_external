<?php

/**
 * simple php SDK for SalsaLabs API
 * verson 1.0 - January 2016
 * author: Jonathan Kissam, jonathankissam.com
 * API documentation: https://help.salsalabs.com/entries/60648624-Salsa-Application-Program-Interface-API-
 */

class SalsaAPI {

	private $node_url = 'PASS_NODE_URL_WHEN_INSTANTIATING_CLASS';
	private $account_email = 'PASS_ACCOUNT_EMAIL_WHEN_INSTANTIATING_CLASS';
	private $passwd = 'PASS_PASSWD_WHEN_INSTANTIATING_CLASS';

	public function __construct($node_url = null, $key = null, $api_key = null) {
		if(!extension_loaded('curl')) trigger_error('SalsaLabs API requires PHP cURL', E_USER_ERROR);
		if(is_null($node_url)) trigger_error('node_url must be supplied', E_USER_ERROR);
		$this->node_url = $node_url;
		if(is_null($account_email)) trigger_error('account_email must be supplied', E_USER_ERROR);
		$this->account_email = $account_email;
		if(is_null($passwd)) trigger_error('passwd must be supplied', E_USER_ERROR);
		$this->passwd = $passwd;
	}

	public function call($endpoint, $method = 'GET', $data = array()) {

	}

	// Salsa API documentation: https://help.salsalabs.com/entries/60648624-Salsa-Application-Program-Interface-API-#save
	public function save_supporter($data = array()) {

	}

}

/*
    $salsa_mapping_lines = explode("\n",$options->salsa_field_mapping);
    foreach ($salsa_mapping_lines as $l) {
      $kv = explode('|',trim($l));
      if (!isset($kv[1]) || !$kv[1]) { $kv[1] = $kv[0]; }
      $salsa_mapping[$kv[0]] = $kv[1];
    }
    // $debug .= "\n\nCivi_mapping:\n\n".print_r($civi_mapping,1);

    $salsa = new SalsaAPI($options->salsa_node_url, $options->salsa_account_email, $options->salsa_password);

    foreach($salsa_mapping as $k => $v) {
      $salsa_data[$k] = isset($submission->data[$cid_mapping[$v]]['value'][0]) ? $submission->data[$cid_mapping[$v]]['value'][0] : '';
    }

    $apifields = array('xml' => 1, 'object' => 'supporter', 'organization_KEY' => $options->salsa_organization_key);
	$salsaGroups = array();
    $callSalsa = false;
    foreach($salsa_data as $k => $v) {
      if ($v) { $apifields[$k] = $v; $callSalsa = true; }
    }

	if ($options->salsa_chapter_key) {
		$apifields['chapter_KEY'] = $options->salsa_chapter_key;
	}

	if ($options->salsa_boolean_field_mapping) {
		$boolean_field_mapping = explode("\n", $options->salsa_boolean_field_mapping);
		foreach ($boolean_field_mapping as $f) {
			$sfs = $submission->data[$cid_mapping[$f]]['value'];
			foreach ($sfs as $sf) {
				$apifields[$sf] = 1;
			}
		}
	}

	if ($options->salsa_groups_field_mapping) {
		$groups_field_mapping = explode("\n", $options->salsa_groups_field_mapping);
		foreach ($groups_field_mapping as $g) {
			$sgs = $submission->data[$cid_mapping[$g]]['value'];
			foreach ($sgs as $sg) {
				$salsaGroups[] = $sg;
			}
		}
	}

	if ($options->salsa_group_id) {
		$salsaGroups[] = $options->salsa_group_id;
	}

	if ($options->salsa_action_key) {
		$apifields['action_KEY'] = $options->action_key;
	}

	if ($options->salsa_autoresponder_ids) {
		$apifields['email_trigger_KEYS'] = $options->salsa_autoresponder_ids;
	}

	if ($options->salsa_tag) {
		$apifields['tag'] = $options->salsa_tag;
	}

	$debug .= "\n\nSalsa mapping:\n\n".print_r($salsa_mapping,1);
	$debug .= "\n\nSalsa data:\n\n".print_r($salsa_data,1);
	$debug .= "\n\napifields:\n\n".print_r($apifields,1);
	$debug .= "\n\nsalsaGroups:\n\n".print_r($salsaGroups,1);

	// while we're developing, don't call Salsa
	// $callSalsa = false;
	$debug .= "\n\nCID Mapping:\n\n".print_r($cid_mapping,1);
	$debug .= "\n\nsubmission:\n\n".print_r($submission,1);
	// mail('jk@webskillet.com','webform external debug',$debug,"From: Firestick Dev <dev@firestick.me>\r\n");

    if ($callSalsa) {
      // $auth = $salsa->authenticate();
      $response = $salsa->call('save',$apifields,$salsaGroups);
    }
	$debug .= "\n\nauth:\n\n".print_r($auth,1);
    $debug .= "\n\nSalsa response:\n\n".print_r($response,1);
*/
