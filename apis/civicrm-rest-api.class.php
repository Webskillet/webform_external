<?php

/**
 * simple php SDK for CIVICRM REST API
 * verson 1.0 - January 2016
 * author: Jonathan Kissam, jonathankissam.com
 * API documentation: http://wiki.civicrm.org/confluence/display/CRMDOC/Using+the+API
 */

class CiviAPI {

	public $restURL = 'PASS_REST_URL_WHEN_INSTANTIATING_CLASS';
	public $key = 'PASS_KEY_WHEN_INSTANTIATING_CLASS';
	public $api_key = 'PASS_API_KEY_WHEN_INSTANTIATING_CLASS';
	public $log = array();

	public function __construct($restURL = null, $key = null, $api_key = null) {
		if(!extension_loaded('curl')) trigger_error('CiviAPI requires PHP cURL', E_USER_ERROR);
		if(is_null($restURL)) trigger_error('rest url must be supplied', E_USER_ERROR);
		if(is_null($key)) trigger_error('key must be supplied', E_USER_ERROR);
		if(is_null($api_key)) trigger_error('api key must be supplied', E_USER_ERROR);
		$this->restURL = $restURL;
		$this->key = $key;
		$this->api_key = $api_key;
	}

	public function call($entity,$action,$params,$method='GET',$returnraw = false) {
		$datafields['entity'] = $entity;
		$datafields['action'] = $action;
		$obj = new stdClass();
		$obj->sequential = 1;
		foreach ($params as $k => $v) {
			$obj->$k = $v;
		}
		$datafields['json'] = json_encode($obj);
		$url = $this->restURL.'?key='.$this->key.'&api_key='.$this->api_key;

		$ch = curl_init();

		// Set basic connection parameters:
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);

		$query_data = str_replace('&amp;','&',http_build_query($datafields));

		if ($method == 'POST') {
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS, $query_data);
		} else {
			$url .= '&'.$query_data;
		}

		curl_setopt($ch, CURLOPT_URL, $url);
		$response = curl_exec($ch);
		$this->log[] = $url .'?'.http_build_query($urlfields);

		curl_close($ch);
		return $returnraw ? $reponse : json_decode($response);

	}

	public function create_contact($params = array()) {

		if (!isset($params['first_name']) && !isset($params['last_name']) && !isset($params['email']) && !isset($params['display_name'])) {
			return 'Error: Mandatory key(s) missing from params array: one of (first_name, last_name, email, display_name)';
		}

		$civi_id = 0;
		$found_by_email = 0;
		$values_obj = new stdClass();

		$simple_params = array();
		foreach ($params as $k => $v) {
			if (!is_array($v)) { $simple_params[$k] = $v; }
		}

		// if we have an email address, make a request to civi to see if they already exist in the database
		if (isset($params['email']) && isset($params['email']['email']) && $params['email']['email']) {
			$request = array(
				'email' => $params['email']['email'],
				'api.website.get' => new stdClass(),
			);
			$response = $this->call('Contact','get',$request);
			// $debug .= "\n\nresponse from call trying to find contact:\n\n".print_r($response,1);
			if ($response->is_error == 0 && ($response->count > 0)) {
				if ($response->count > 1) {
					$debug .= "\n\nMultiple matches; here's response->values object:\n\n".print_r($response->values, 1);
				}
				$values_arr = (array) $response->values;
				$values_arr = array_values($values_arr);
				$values_obj = $values_arr[0];
				$civi_id = isset($values_obj->contact_id) ? $values_obj->contact_id : 0;
				$found_by_email = $civi_id ? 1 : 0;
			}
		}
		if ($civi_id) {
			$simple_params['contact_id'] = $civi_id;
			// $debug .= "\n\nFound Civi record id $civi_id for email ".$params['email']['email'].".";
		}

		$response = $this->call('Contact','create',$simple_params,'POST');
		$civi_id = ($response->is_error == 0) ? $response->id : 0;
		$debug .= "\n\nResponse from account create call:\n\n".print_r($response, 1);

		if (!$civi_id) { return; }

	    // record email if we didn't find an existing record that way
		if (!$found_by_email && isset($params['email']) && isset($params['email']['email'])) {
			$params['email']['contact_id'] = $civi_id;
			$response = $this->call('Email','create',$params['email'],'POST');
			// $debug .= "\n\nresponse from email create call:\n\n".print_r($response,1);
		}

	    // compare submitted phone number with existing phone number
		$phoneok = (isset($values_obj->phone) && isset($params['phone']) && isset($params['phone']['phone'])
			&& $this->compare_phones($values_obj->phone, $params['phone']['phone']) );
		if (isset($params['phone']) && isset($params['phone']['phone']) && !$phoneok) {
			$params['phone']['contact_id'] = $civi_id;
			$response = $this->call('Phone','create',$params['phone'],'POST');
			// $debug .= "\n\nresponse from phone create call:\n\n".print_r($response,1);
		}

		// compare submitted address with existing address
		if (isset($params['address'])) {
			$address = $this->compare_address($values_obj, $params['address']);
			// $debug .= "\n\nvalues_obj:\n\n".print_r($values_obj,1)."\n\nparams:\n\n".print_r($params,1);
			if (!isset($address['ok'])) {
				$address['contact_id'] = $civi_id;
				$response = $this->call('Address','create',$address,'POST');
				// $debug .= "\n\nresponse from address create call:\n\n".print_r($response,1);
			}
		}

		// compare submitted website with existing website
		if (isset($params['website']) && isset($params['website']['url'])) {
			$websiteok = isset($values_obj->{'api.website.get'}->values[0]->url) ? $this->compare_string($values_obj->{'api.website.get'}->values[0]->url, $params['website']['url']) : 0;
			// $debug .= "\n\nvalues_obj:\n\n".print_r($values_obj,1)."\n\nparams:\n\n".print_r($params,1);
			if (!$websiteok) {
				$params['website']['contact_id'] = $civi_id;
				$response = $this->call('Website','create',$params['website'],'POST');
				// $debug .= "\n\nresponse from website create call:\n\n".print_r($response,1);
			}
		}

		if (isset($params['EntityTag'])) {
			foreach ($params['EntityTag'] as $tag_id) {
				$response = $this->call('EntityTag','create',array(
					'tag_id' => $tag_id,
					'entity_table' => 'civicrm_contact',
					'entity_id' => $civi_id,
				),'POST');
				// $debug .= "\n\nresponse from tag create call:\n\n".print_r($response,1);
			}
		}

		if (isset($params['GroupContact'])) {
			foreach ($params['GroupContact'] as $group_id) {
				$response = $this->call('GroupContact','create',array(
					'group_id' => $tag_id,
					'contact_id' => $civi_id,
				),'POST');
				// $debug .= "\n\nresponse from group create call:\n\n".print_r($response,1);
			}
		}

		return $civi_id;

	}


	// compares phone numbers by pulling out all formatting
	private function compare_phones($n1, $n2) {
		$n1clean = preg_replace('/[^0-9]/','',$n1);
		$n2clean = preg_replace('/[^0-9]/','',$n2);
		return ($n1clean == $n2clean);
	}

	// compares addresses and returns an object with submitted data
	private function compare_address($obj, $data) {
		$address = array();
		$ok = 1;

		foreach(array('street_address','supplemental_address_1','supplemental_address_2','city','state_province','postal_code') as $n) {
			$obj->$n = isset($obj->$n) ? $obj->$n : '';
			$data[$n] = isset($data[$n]) ? $data[$n] : '';
		}

		if ($data['street_address'] &&
			!$this->compare_string($obj->street_address,$data['street_address'],6)) { $ok = 0; }
		if ($data['supplemental_address_1'] &&
			!$this->compare_string($obj->supplemental_address_1,$data['supplemental_address_1'],6)) { $ok = 0; }
		if ($data['supplemental_address_2'] &&
			!$this->compare_string($obj->supplemental_address_2,$data['supplemental_address_2'],6)) { $ok = 0; }
		if ($data['city'] &&!$this->compare_string($obj->city,$data['city'])) { $ok = 0; }
		if ($data['state_province'] && ($obj->state_province != $data['state_province'])) { $ok = 0; }
		if ($data['postal_code'] && !$this->compare_string($obj->postal_code,$data['postal_code'],6)) { $ok = 0; }

		if ($ok) { $data['ok'] = true; }
		return $data;
	}

	private function compare_string($s1,$s2,$n=0) {
		// to lowercase and remove anything that's not alphanumeric
		$s1 = strtolower($s1);
		$s2 = strtolower($s2);
		$s1 = preg_replace('/[^a-z0-9]/','',$s1);
		$s2 = preg_replace('/[^a-z0-9]/','',$s2);
		if ($n > 0) {
			$s1 = substr($s1,0,$n);
			$s2 = substr($s2,0,$n);
		}
		return ($s1 == $s2);
	}

}


/**

To add a contact with phones, addresses, websites, groups and/or tags using chained API:

https://ndwa.ourpowerbase.net/sites/all/modules/civicrm/extern/rest.php?key=[key]&api_key=[api_key]&action=create&entity=Contact&json=

{
	"sequential":1,
	"contact_type":"Individual",
	"first_name":"API",
	"last_name":"Test",
	"email":"jonathan%2Bapitest9.25.2015.2",
	"api.phone.create":{
		"phone":"%2B18028258363",
		"phone_type_id":"2"
	},
	"api.address.create":{
		"street_address":"19 Church Street",
		"supplemental_address_1":"Suite 10",
		"city":"Burlington",
		"state_province":"VT",
		"postal_code":"05401",
		"location_type_id":"1"
	},
	"api.website.create":}
		"url":"http://jonathankissam.com",
		"website_type_id":"2"
	},
	"api.GroupContact.create":{
		"group_id":"139"
	}
	"api.EntityTag.create":{
		"tag_id":"139"
	}
}

*/
