<?php  
defined('SMSPanel') or die('Snooping around is not allowed. Please use the front door');
session_start();
$clientArea = false;
define('clientArea',false);
error_reporting(0);  
//set time zone
if(!function_exists('getSlash')) {
	function getSlash() {
		$sSoftware = strtolower( $_SERVER["SERVER_SOFTWARE"] );
		if ( strpos($sSoftware, "microsoft-iis") !== false )
			return "\\";
		else
			return "/";
	}
}

//set user ip
$IP = getenv('REMOTE_ADDR');
//set current year 
$current_year = date('Y');
//Set root directory index
	if(!INDEX) {
	  define('BASE', '');
	}
	if(INDEX == 1) {
	  define('BASE', '../');
	}
	if(INDEX == 2) {
	  define('BASE', '../../');
	}
	if(INDEX == 3) {
	  define('BASE', '../../../');
	}		
//Call path finder	
include_once(BASE.'root.php');		
$page = $_SERVER['REQUEST_URI'];
$currentFile = str_replace('//','/',$page);

//Include All External PHP resources
include_once(DOCUMENT_ROOT.'drivers/includes/config.inc');
require(DOCUMENT_ROOT.'drivers/includes/Services/Twilio.php');
$state=$sconfig['installed'];if($state!='INSTALLED') { 
header('location: '.BASE.'install');
}
//setup loaders
$path='/components/';	
$file='.php';
//initiate connection 
	mysql_connect($sconfig['host'],$sconfig['user'],$sconfig['password']);
	mysql_select_db($sconfig['database']) or die( "Unable to connect to database. Please contact the webmaster ");
	
//load settings
function getSetting($field) {
	$query="SELECT * FROM settings WHERE field = '$field'"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$value = $row['value'];	
	if(empty($value)) {
	return false;
	} else {
	return $value;
	}
}
//load defaults 
$siteName = getSetting('siteName');
$siteLogo = getSetting('siteLogo');
if(empty($siteName)): $siteName = 'Sendroid v1.0'; endif;
$siteLogo = 'media/images/logo.png'; 
//set time znes
$timeString='258358845643583548584535252652';
$serverTimeZone = getSetting('defaultTimeZone');
date_default_timezone_set($serverTimeZone);
$tz = date_default_timezone_get();
//call all functions
include_once(DOCUMENT_ROOT.APP_PATHS);	
include_once(DOCUMENT_ROOT.'plugins/functions.inc');
if(isLogedIn()) : 
$userTimeZone = userData('username','timeZone');
endif;
?>	