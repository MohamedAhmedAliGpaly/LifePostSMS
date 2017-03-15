<?php  
defined('SMSPanel') or die('Snooping around is not allowed. Please use the front door');
session_start();
$clientArea = true;
define('clientArea',true);
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
$currentFile = $page; //str_replace('/SMSPanel','',$page);

//Include All External PHP resources
include_once(DOCUMENT_ROOT.'drivers/includes/config.inc');
require DOCUMENT_ROOT.'drivers/includes/Stripe/lib/ini.php';
require(DOCUMENT_ROOT.'drivers/includes/Services/Twilio.php');
$state=$sconfig['installed'];if($state!='INSTALLED') { 
header('location: '.BASE.'install');
}
//setup loaders
if( (substr(dirname( __FILE__ ), -1) == '/') || (substr(dirname( __FILE__ ), -1) == '\\') ) {
	$path='components/';	
} else {
	$path='/components/';
}


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

function getResellerSetting($field,$reseller) {
	$query="SELECT * FROM resellers WHERE customer = '$reseller'"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$value = $row[$field];	
	if(empty($value)) {
	return false;
	} else {
	return $value;
	}
}

include_once(DOCUMENT_ROOT."drivers/Facebook/config.php");

//check for reseller
$reseller = 0;
$source_domain = get_domain();
$query="SELECT * FROM resellers WHERE allowed_domain LIKE '%$source_domain%'"; 
$result = mysql_query($query) or die(mysql_error());  
$found = mysql_num_rows($result);
if($found < 1) {
	$reseller = 0;
} else {
	$row = mysql_fetch_assoc($result); 
	$reseller = $row['customer'];	
}

//load defaults 
$siteName = getSetting('siteName');
$logo = getSetting('siteLogo');
$siteLogo = 'media/uploads/'.getSetting('siteLogo');
$serverTimeZone = getSetting('defaultTimeZone');

if($reseller > 0) {
	$siteName = getResellerSetting('business_name',$reseller);
	$logo = getResellerSetting('logo_url',$reseller);
	$siteLogo = 'media/uploads/'.getResellerSetting('logo_url',$reseller);
	$serverTimeZone = getResellerSetting('default_timezone',$reseller);
}

if(empty($siteName)): $siteName = 'SMSPanel v3.0'; endif;
if(empty($logo)): $siteLogo = 'media/images/logo.png'; endif;
//set time znes
date_default_timezone_set($serverTimeZone);
$tz = date_default_timezone_get();
//call all functions
include_once(DOCUMENT_ROOT.APP_PATHS);	
include_once(DOCUMENT_ROOT.'plugins/functions.inc');
if(isLogedIn()) : 
$UserID = getUser();
$userTimeZone = userData('timeZone',getUser());
date_default_timezone_set($serverTimeZone);
$tz = date_default_timezone_get();
endif;

?>	