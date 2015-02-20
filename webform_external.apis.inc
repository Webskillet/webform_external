<?php

/**
 * @file
 * APIs for external webform integration
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

	public function call($entity,$action,$fields,$returnraw = false) {
		$urlfields['json'] = 1;
		$urlfields['entity'] = $entity;
		$urlfields['action'] = $action;
		$urlfields = array_merge($urlfields,$fields);
		$urlfields['key'] = $this->key;
		$urlfields['api_key'] = $this->api_key;

		$ch = curl_init();
		$url = $this->restURL;

		// Set basic connection parameters:
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);

		curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($urlfields));
		curl_setopt($ch, CURLOPT_URL, $url);
		$response = curl_exec($ch);
		$this->log[time()] = $url.'?'.http_build_query($urlfields);

		curl_close($ch);
		return $returnraw ? $reponse : json_decode($response);

	}

}

class salsaAPI {

	public $email = 'PASS_EMAIL_WHEN_INSTANTIATING_CLASS';
	public $passwd = 'PASS_PASSWORD_WHEN_INSTANTIATING_CLASS';
	public $nodeURL = 'PASS_NODE_URL_WHEN_INSTANTIATING_CLASS';

	public function __construct($nodeURL = null, $email = null, $passwd = null) {
		if(!extension_loaded('curl')) trigger_error('salsaAPI requires PHP cURL', E_USER_ERROR);
		if(is_null($email)) trigger_error('email must be supplied', E_USER_ERROR);
		if(is_null($passwd)) trigger_error('password must be supplied', E_USER_ERROR);
		if(is_null($nodeURL)) trigger_error('Salsa node URL must be supplied', E_USER_ERROR);
		$this->email = $email;
		$this->passwd = $passwd;
		$this->nodeURL = $nodeURL;
	}

	/* general API call functions */

	public function authenticate() {
		$ch = curl_init();

		$authfields["email"] = $this->email;
		$authfields["password"] = $this->passwd;
		$url = $this->nodeURL;

		// Set basic connection parameters:
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);

		// Set Parameters to maintain cookies across sessions
		curl_setopt($ch, CURLOPT_COOKIESESSION, TRUE);
		curl_setopt($ch, CURLOPT_COOKIEFILE, '/tmp/cookies_file');
		curl_setopt($ch, CURLOPT_COOKIEJAR, '/tmp/cookies_file');

		curl_setopt($ch, CURLOPT_URL, "$url/api/authenticate.sjs");
		curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($authfields));
		$auth = curl_exec($ch);

		curl_close($ch);
		return $auth;
	}

	public function call($method, $fields, $returnraw = false) {
		$ch = curl_init();

		$url = $this->nodeURL;

		// Set basic connection parameters:
		curl_setopt($ch, CURLOPT_POST, 1);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);

		// Set Parameters to maintain cookies across sessions
		curl_setopt($ch, CURLOPT_COOKIESESSION, TRUE);
		curl_setopt($ch, CURLOPT_COOKIEFILE, '/tmp/cookies_file');
		curl_setopt($ch, CURLOPT_COOKIEJAR, '/tmp/cookies_file');

		curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($fields));
		curl_setopt($ch, CURLOPT_URL, "$url/$method");
		$response = curl_exec($ch);

		curl_close($ch);
		return $returnraw ? $reponse : $this->response_to_xml($response);
	}

	private function response_to_xml($response) {
		$xml = simplexml_load_string($response);
		return $xml;
	}

}


