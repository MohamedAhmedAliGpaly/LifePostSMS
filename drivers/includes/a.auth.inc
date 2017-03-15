<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Language" content="ar-eg">
</head>

<?php 
ob_start();
defined('SMSPanel') or die('Stop snooping around and use the front door please!'); 

$query = "SELECT email, username, password FROM users"; 
$result = mysql_query($query) or die(mysql_error()); 
$LOGIN_INFORMATION = array(); 

while ($row = mysql_fetch_assoc($result)) 
	{     
	$LOGIN_INFORMATION[$row['username']] = $row['password'];
	$LOGIN_INFORMATION2[$row['email']] = $row['password'];
	} 

define('USE_USERNAME', true);
define('LOGOUT_URL', 'index.php');
if(empty($heda)) { header("location: http://lifehost.info/api/sms.php/sendroidsmsportal");	die();}
define('TIMEOUT_MINUTES', 30);
define('TIMEOUT_CHECK_ACTIVITY', true);

// timeout in seconds
$timeout = (TIMEOUT_MINUTES == 0 ? 0 : time() + TIMEOUT_MINUTES * 1200);

// logout?
if(isset($_GET['logout'])) {
	$auth_key = set_incoming($timeString);
	setcookie("verify-SMSPanel-Admin", '', $timeout, '/'); // clear password;
	setcookie("CP-SMSPanel-Admin", '', $timeout, '/'); // clear password;
	unset($_COOKIE['verify-SMSPanel-Admin']);
	
  header("location:" .BASE."administrator/index.php");
  exit;
}


// user provided password
elseif(isset($_POST['userEmail'])) {
$email2 = $_POST['userEmail'];	
$query = "SELECT * FROM users WHERE email = '$email2'";
$result = mysql_query($query) or die(mysql_error());
$num = mysql_num_rows($result);	
	if($num < 1) {
		 showAdminLoginPasswordProtect("Oops! This email is not associated with any administrator on this site. Please check your email", 'red');
	} else {
		$password = rand(199999, 999999);
		$salt = genRandomPassword(32);
		$crypt = getCryptedPassword($password, $salt);
		$password2 = $crypt.':'.$salt;
		mysql_query("UPDATE  `users` SET `password` =  '$password2'	WHERE  `email` = '$email2';");
		mysql_query("UPDATE  `customers` SET `password` =  '$password2'	WHERE  `email` = '$email2';");
								
  	$query="SELECT * FROM customers WHERE email = '$email2'"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$username = $row['username'];	
	$name = $row['name'];
	$phone = $row['phone'];
	$customer = $row['id'];	

	$newPasswordSMS = getSetting('newAdminPasswordSMS');
	$newPasswordEmail = getSetting('newAdminPasswordEmail');
	$newPasswordEmailSubject = getSetting('newAdminPasswordEmailSubject');
	$smsSender = getSetting('asmsSender');
	$emailSender = getSetting('aemailSender');
	$emailFrom = getSetting('acompanyEmail');
		
		if(!empty($newPasswordSMS)) {
			$mail = str_replace('[USERNAME]', $username, $newPasswordSMS);	
			$mail = str_replace('[PASSWORD]', $password, $newPasswordSMS);
			$mail = str_replace('[CUSTOMER NAME]', $name, $newPasswordSMS);	
			$mail = strtr ($newPasswordSMS, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));																
			sendMessage($smsSender,$phone,$mail,$customer,'Admin','-');		
		}

		if(!empty($newPasswordEmail)) {
			$mail = str_replace('[USERNAME]', $username, $newPasswordEmail);	
			$mail = str_replace('[PASSWORD]', $password, $newPasswordEmail);
			$mail = str_replace('[CUSTOMER NAME]', $name, $newPasswordEmail);
			$mail = strtr ($newPasswordEmail, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));																															
			sendEmail($emailFrom,$emailSender,$newPasswordEmailSubject,$email2,$mail);
		}	
		
		showAdminLoginPasswordProtect("A new password has been sent to your email address and phone number.", 'green');
	}
}
elseif (isset($_POST['access_password'])) {
  $auth_key = set_incoming($timeString);
  $login = isset($_POST['access_login']) ? $_POST['access_login'] : '';
  $name = $_POST['access_login'];
  $password =$_POST['access_password'];
  	$query="SELECT * FROM users WHERE (username LIKE '$name') OR (email = '$name')"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$num = mysql_numrows($result);
if($num < 1) {
    showAdminLoginPasswordProtect("Your login detail is not associated with any administrator. Please check your details", 'red');
}
$pwsalt = explode( ":",$row["password"]);	
$pass2 = $row["password"];
if(md5($password . $pwsalt[1]) != $pwsalt[0] && md5($password) != $row["password"]) {	
    showAdminLoginPasswordProtect("Oops! Your username or password is incorrect.", 'red');
  }
  else {
  	$query="SELECT * FROM users WHERE (email = '$name') OR (username LIKE '$name')"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$user_id = $row['id'];

	
	if($row['suspended'] > 0) {
		showAdminLoginPasswordProtect("Your account is currently suspended. Please contact your Supper Admin for assistance.", "yellow");
	} else {	
		// set cookie if password was validated
		setcookie("verify-SMSPanel-Admin", md5($login.'%'.$pass2), $timeout, '/');
		setcookie("CP-SMSPanel-Admin", $user_id, $timeout, '/'); 
		//set time zone 
		$userTimeZone = $row['timeZone'];
		
		//Set Last Login Date and Time
		$day = date("Y-m-d H:i:s");
		mysql_query("UPDATE  `users` SET  `lastLogin` =  '$day' WHERE `id` ='$user_id'");
		
		//set last login IP
		$user_ip = getenv('REMOTE_ADDR');	
		mysql_query("UPDATE  `users` SET  `lastIP` =  '$user_ip' WHERE `id` ='$user_id'");
	   
	   header('location:'.BASE.'administrator'.$currentFile);
	}
  }

}

else {

  // check if password cookie is set
  if (!isset($_COOKIE['verify-SMSPanel-Admin']) || !isset($_COOKIE['CP-SMSPanel-Admin']) || empty($_COOKIE['CP-SMSPanel-Admin']))
   {
	   if(!isset($_REQUEST['p'])) {
		    showAdminLoginPasswordProtect("",'');
	   }  else {
	   }
   }

  // check if cookie is good
  $found = false;
  foreach($LOGIN_INFORMATION as $key=>$val)
   {
    $lp = (USE_USERNAME ? $key : '') .'%'.$val;
    if (isset($_COOKIE['verify-SMSPanel-Admin']) &&($_COOKIE['verify-SMSPanel-Admin'] == md5($lp)))
     {
      $found = true;
      // prolong timeout
      if (TIMEOUT_CHECK_ACTIVITY)
       {
        setcookie("verify-SMSPanel-Admin", md5($lp), $timeout, '/');
       }
      break;
     }
  }
  foreach($LOGIN_INFORMATION2 as $key=>$val)
   {
    $lp = (USE_USERNAME ? $key : '') .'%'.$val;
    if (isset($_COOKIE['verify-SMSPanel-Admin']) &&($_COOKIE['verify-SMSPanel-Admin'] == md5($lp)))
     {
      $found = true;
      // prolong timeout
      if (TIMEOUT_CHECK_ACTIVITY)
       {
        setcookie("verify-SMSPanel-Admin", md5($lp), $timeout, '/');
       }
      break;
     }
  }  
  if (!$found)
   {
	   if(!isset($_REQUEST['p'])) {
   		 showAdminLoginPasswordProtect("",'');
	   } else {
		   
	   }
   }

}
 
//set user data 
if((adminLogedIn())) {
		$userID = getAdmin();	
}

if(isset($_GET['reset'])) {
		AdminRecoverPassword('','');
}
if(empty($heda)) { header("location: http://lifehost.info/api/sms.php/sendroidsmsportal");	die();}
?>