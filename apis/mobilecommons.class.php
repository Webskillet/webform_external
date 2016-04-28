<?php

/**
 * simple php SDK for Mobile Commons API
 * verson 1.0 - January 2016
 * author: Jonathan Kissam, jonathankissam.com
 * API documentation: https://mobilecommons.zendesk.com/hc/en-us/articles/202052534-REST-API
 */

class MobileCommons {

	private $username = 'PASS_USERNAME_WHEN_INSTANTIATING_CLASS';
	private $passwd = 'PASS_USERNAME_WHEN_INSTANTIATING_CLASS';
	private $subdomain = 'https://secure.mcommons.com';

	public function __construct($username = null, $passwd = null, $subdomain = 'https://secure.mcommons.com') {
		if(!extension_loaded('curl')) trigger_error('MobileCommons requires PHP cURL', E_USER_ERROR);
		if(is_null($username)) trigger_error('username must be supplied', E_USER_ERROR);
		$this->username = $username;
		if(is_null($passwd)) trigger_error('passwd must be supplied', E_USER_ERROR);
		$this->passwd = $passwd;
		$this->subdomain = $subdomain;
	}

	public function call($endpoint, $method = 'GET', $parameters = array()) {

		$url = $this->subdomain.'/api/'.$endpoint;
		$parameters_query = str_replace('&amp;','&',http_build_query($parameters));

		$ch = curl_init();
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_TIMEOUT, 100);
		curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
		curl_setopt($ch, CURLOPT_USERPWD, "{$this->username}:{$this->passwd}");
		if ($method == "POST") {
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS, $parameters_query);
		} else {
			if ($parameters_query) { $url .= '?'.$parameters_query; }
		}
		curl_setopt($ch, CURLOPT_URL, $url);

		$response = curl_exec($ch);

		curl_close($ch);

		return $response;
	}

	public function profile_update($parameters = array()) {

		if (!isset($parameters['phone_number'])) trigger_error('phone_number must be supplied for MobileCommons->profile_update', E_USER_ERROR);
		return $this->call('profile_update','POST',$parameters);

	}

}
