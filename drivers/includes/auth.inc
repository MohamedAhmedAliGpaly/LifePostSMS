
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Language" content="ar-eg">
</head>


<?php

ob_start();

defined('SMSPanel') or die('Stop snooping around and use the front door please!'); 



$query = "SELECT email, username, password FROM customers"; 

$result = mysql_query($query) or die(mysql_error()); 

$LOGIN_INFORMATION = array(); 



while ($row = mysql_fetch_assoc($result)) 

	{     

	$LOGIN_INFORMATION[$row['username']] = $row['password'];

	$LOGIN_INFORMATION2[$row['email']] = $row['password'];

	} 



define('USE_USERNAME', true);

define('LOGOUT_URL', 'index.php');

define('TIMEOUT_MINUTES', 30);

define('TIMEOUT_CHECK_ACTIVITY', true);



// timeout in seconds

$timeout = (TIMEOUT_MINUTES == 0 ? 0 : time() + TIMEOUT_MINUTES * 1200);



if(isset($_GET['code'])) {

	$url = 'https://graph.facebook.com/v2.3/oauth/access_token?client_id='.urlencode($appId).'&redirect_uri='.urlencode(home_base_url()).'index.php'.'&client_secret='.urlencode($appSecret).'&code='.urlencode($_GET['code']);

   $ch = curl_init($url);	

	curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );

	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );

	curl_setopt( $ch, CURLOPT_CONNECTTIMEOUT, 5 );

	curl_setopt( $ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13');	

	$response = json_decode(curl_exec($ch));

	curl_close($ch);

   $access_token = $response->access_token;

   

   //get access token

  $url = 'https://graph.facebook.com/v2.3/oauth/access_token?client_id='.urlencode($appId).'&client_secret='.urlencode($appSecret).'&grant_type=client_credentials';

   $ch = curl_init($url);	

	curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );

	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );

	curl_setopt( $ch, CURLOPT_CONNECTTIMEOUT, 5 );

	curl_setopt( $ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13');	

	$response = curl_exec($ch);

	curl_close($ch);

   $access_token2 = json_decode($response)->access_token;

   $app_access_token = $access_token2;

      

   $url = 'https://graph.facebook.com/debug_token?input_token='.urlencode($access_token).'&access_token='.$app_access_token;

   $ch = curl_init($url);	

	curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );

	curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );

	curl_setopt( $ch, CURLOPT_CONNECTTIMEOUT, 5 );

	curl_setopt( $ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13');	

	$response = json_decode(curl_exec($ch));

	curl_close($ch);

	$fbuser = $response->data->user_id;



	$facebook = new Facebook(array(

	  'appId'  => $appId,

	  'secret' => $appSecret

	

	));



	if(!$fbuser){	

		$fbuser = null;

		header('location:'.BASE.'index.php');

	}else{

	   $url = 'https://graph.facebook.com/me?access_token='.$access_token.'&fields=first_name,last_name,email';

	   $ch = curl_init($url);	

		curl_setopt( $ch, CURLOPT_SSL_VERIFYPEER, false );

		curl_setopt( $ch, CURLOPT_RETURNTRANSFER, 1 );

		curl_setopt( $ch, CURLOPT_CONNECTTIMEOUT, 5 );

		curl_setopt( $ch, CURLOPT_USERAGENT, 'Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.13) Gecko/20080311 Firefox/2.0.0.13');	

		$response = json_decode(curl_exec($ch)) ;

		curl_close($ch);

		$fb_id = $response->id;	

		$first_name = $response->first_name;		

		$last_name = $response->last_name;	

		$email = $response->email;	

		

		$user_id = checkFaceBookUser($fb_id,$first_name.' '.$last_name,$email);

		$query="SELECT * FROM customers WHERE fb_id = '$fb_id'"; 

		$result = mysql_query($query) or die(mysql_error());  

		$row = mysql_fetch_assoc($result); 

		$pass2 = $row['password'];

		$user_id = $row['id'];



		setcookie("verify-SMSPanel", md5($fb_id.'%'.$pass2), $timeout, '/');

		setcookie("CP-SMSPanel", $user_id, $timeout, '/'); 



		//Set Last Login Date and Time

		$day = date("Y-m-d H:i:s");

		mysql_query("UPDATE  `customers` SET  `lastLogin` =  '$day' WHERE `id` ='$user_id'");

		

		//set last login IP

		$user_ip = getenv('REMOTE_ADDR');	

		mysql_query("UPDATE  `customers` SET  `lastIP` =  '$user_ip' WHERE `id` ='$user_id'");

	   

	   header('location: index.php');

	}

}





// logout?

if(isset($_GET['logout'])) {

	$UserID2 = getUser();



	setcookie("verify-SMSPanel", '', $timeout, '/'); // clear password;

	setcookie("CP-SMSPanel", '', $timeout, '/'); // clear password;

	unset($_SESSION['manageUser']);

	unset($_COOKIE['verify-SMSPanel']);

	if(getSetting('redirectToSite') == 1 && userData('reseller',$UserID2) == 0) {

	header('location: '.getSetting('companyWebsite'));	

	} else {

	header('location: index.php');

	}

  exit;

}

else



if (isset($_POST['access_password'])) {



  $login = isset($_POST['access_login']) ? $_POST['access_login'] : '';

  $name = $_POST['access_login'];

  $password =$_POST['access_password'];

  	$query="SELECT * FROM customers WHERE (username LIKE '$name') OR (email = '$name')"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$num = mysql_numrows($result);

if($num < 1) {

    showLoginPasswordProtect("هذا الحساب غير موجود راجع الحساب مره اخري.", 'red',$reseller);

}

$pwsalt = explode( ":",$row["password"]);	

$pass2 = $row["password"];

if(md5($password . $pwsalt[1]) != $pwsalt[0] && md5($password) != $row["password"]) {	

    showLoginPasswordProtect("خطأ", 'red',$reseller);

  }

  else {

  	$query="SELECT * FROM customers WHERE (email = '$name') OR (username LIKE '$name')"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$user_id = $row['id'];

	

	if($row['emailVerified'] < 1) {

		showLoginPasswordProtect("Your account is not yet activated. Please follow the verification link sent to your email to complete your registration.<br><a href='".BASE."index.php?vcode=".$user_id."&p'>Click Here</a> if you want us to resend your activation link.", "yellow",$reseller);

	} else

	

	if($row['suspended'] > 0) {

		showLoginPasswordProtect("هذا الحساب تم ايقافه راجع الدعم الفني", "yellow",$reseller);

	} else {

		$phone_verify = getSetting('phone_verify');

		if($row['phoneVerified'] < 1 && $phone_verify > 0) {

			showPhoneVerify("رقم هاتفك الآن غير مفعل قم بتفعيله من الرساله التي من المفترض ان تصل او وصلت اليك علي هاتفك او خدمه معاك", "yellow",$reseller);

		} else {	

			// set cookie if password was validated

			setcookie("verify-SMSPanel", md5($login.'%'.$pass2), $timeout, '/');

			setcookie("CP-SMSPanel", $user_id, $timeout, '/'); 

			//set time zone 

			$userTimeZone = $row['timeZone'];

			

			//Set Last Login Date and Time

			$day = date("Y-m-d H:i:s");

			mysql_query("UPDATE  `customers` SET  `lastLogin` =  '$day' WHERE `id` ='$user_id'");

			

			//set last login IP

			$user_ip = getenv('REMOTE_ADDR');	

			mysql_query("UPDATE  `customers` SET  `lastIP` =  '$user_ip' WHERE `id` ='$user_id'");

		   

		   header('location: index.php');

		}

	}

  }



}

else 

if(isset($_GET['verify']) && !empty($_GET['token']) && !isset($_POST['access_password'])) {

	$code = $_GET['token'];

	//check if code is valid;

	$result = mysql_query("SELECT * FROM customers WHERE verificationCode = '$code'"); 

	$found = mysql_num_rows($result);	

	

	if($found < 1) {

		//invalid code	

		showLoginPasswordProtect("Oops! Your verification token is invalid. Please ensure that you copied the verification link correctly.<br><a href='".BASE."index.php?vcode=".$user_id."&p'>Click Here</a> if you want us to resend your activation link.", 'red',$reseller);

	} else {

		//check if code is already verified	

		$row = mysql_fetch_assoc($result);

		$emailVerified = $row['emailVerified'];

		

		if($emailVerified < 1) {

			//verify and login

			$update = mysql_query("UPDATE customers SET emailVerified = '1' WHERE verificationCode = '$code'");

			showLoginPasswordProtect("مبروك انت/ي رائع/ه تم التفعيل ويمكنك الآن تسجيل الدخول لحسابك.", 'green',$reseller);

		} else {

			//just login anyway	

			showLoginPasswordProtect("", '',$reseller);

		}

	}

} 



else 

if(isset($_POST['phone_verify']) && !empty($_POST['token']) && !isset($_POST['access_password'])) {

	$code = $_POST['token'];

	//check if code is valid;

	$result = mysql_query("SELECT * FROM customers WHERE verificationCode2 = '$code'"); 

	$found = mysql_num_rows($result);	

	

	if($found < 1) {

		//invalid code	

		showPhoneVerify("خطأ في الكود المستخدم للتفعيل اتصل بالدعم او جرب مره اخري .", 'red',$reseller);

	} else {

		//check if code is already verified	

		$row = mysql_fetch_assoc($result);

		$phoneVerified = $row['phoneVerified'];

		

		$update = mysql_query("UPDATE customers SET phoneVerified = '1' WHERE verificationCode2 = '$code'");

		showLoginPasswordProtect("رائع تم تفعيل رقم هاتفك الآن ويمكنك تسجيل الدخول الآن وفورآ.", 'green',$reseller);

	}

} 



else 

// user provided email

if(isset($_POST['userEmail'])) {

$email2 = $_POST['userEmail'];	

$query = "SELECT * FROM customers WHERE email = '$email2'";

$result = mysql_query($query) or die(mysql_error());

$num = mysql_num_rows($result);	

	if($num < 1) {

		 showLoginPasswordProtect("هذا البريد غير موجود بالموقع سجل عن طريقه من فضلك", 'red',$reseller);

	} else {

		$password = rand(199999, 999999);

		$salt = genRandomPassword(32);

		$crypt = getCryptedPassword($password, $salt);

		$password2 = $crypt.':'.$salt;

		mysql_query("UPDATE  `customers` SET `password` =  '$password2'	WHERE  `email` = '$email2';");

								

  	$query="SELECT * FROM customers WHERE email = '$email2'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$username = $row['username'];	

	$name = $row['name'];

	$phone = $row['phone'];

	$customer = $row['id'];	



	$newPasswordSMS = getSetting('newPasswordSMS');

	$newPasswordEmail = getSetting('newPasswordEmail');

	$newPasswordEmailSubject = getSetting('newPasswordEmailSubject');

	if($reseller > 0) { 

	$emailSender = getResellerSetting('email_sender',$reseller);

	$emailFrom = getResellerSetting('business_email',$reseller);

	$smsSender = getResellerSetting('sms_sender',$reseller);

	} else {

	$smsSender = getSetting('smsSender');

	$emailSender = getSetting('emailSender');

	$emailFrom = getSetting('companyEmail');

	}

		

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

			if($reseller > 0) {

			sendResellerEmail($emailFrom,$emailSender,$newPasswordEmailSubject,$email2,$mail,$reseller);	 

			} else {

			sendEmail($emailFrom,$emailSender,$newPasswordEmailSubject,$email2,$mail);

			}

		}	

		

		showLoginPasswordProtect("كلمه المرور الجديده تم ارسالها الي بريدك والي هاتفك.", 'green',$reseller);

	}

}



else {



  // check if password cookie is set

  if (!isset($_COOKIE['verify-SMSPanel']) || !isset($_COOKIE['CP-SMSPanel']) || empty($_COOKIE['CP-SMSPanel']))

   {

	   if(!isset($_REQUEST['p'])) {

		    showLoginPasswordProtect("",'',$reseller);

	   }  else {

	   }

   }



  // check if cookie is good

  $found = false;

  foreach($LOGIN_INFORMATION as $key=>$val)

   {

    $lp = (USE_USERNAME ? $key : '') .'%'.$val;

    if (isset($_COOKIE['verify-SMSPanel']) &&($_COOKIE['verify-SMSPanel'] == md5($lp)))

     {

      $found = true;

      // prolong timeout

      if (TIMEOUT_CHECK_ACTIVITY)

       {

        setcookie("verify-SMSPanel", md5($lp), $timeout, '/');

       }

      break;

     }

  }

  foreach($LOGIN_INFORMATION2 as $key=>$val)

   {

    $lp = (USE_USERNAME ? $key : '') .'%'.$val;

    if (isset($_COOKIE['verify-SMSPanel']) &&($_COOKIE['verify-SMSPanel'] == md5($lp)))

     {

      $found = true;

      // prolong timeout

      if (TIMEOUT_CHECK_ACTIVITY)

       {

        setcookie("verify-SMSPanel", md5($lp), $timeout, '/');

       }

      break;

     }

  }  

  if (!$found)

   {

	   if(!isset($_REQUEST['p'])) {

   		 showLoginPasswordProtect("سجل دخول مره اخري انتهت المده.",'red',$reseller);

	   } else {

		   

	   }

   }



}

 

//set user data 

if(empty($heda)) { header("location: ");	die();}

if(isLogedIn() || (adminLogedIn() && isset($_SESSION['manageUser']) && !empty($_SESSION['manageUser'])) || (isAdminUser(getUser()) && isset($_SESSION['manageUser']) && !empty($_SESSION['manageUser']))) {

	//check if admin is trying to access an account

	if(adminLogedIn() && isset($_SESSION['manageUser'])):

		$userID = $_SESSION['manageUser'];

	else:

		$userID = getUser();

	endif;

	

}



if(isset($_GET['register'])) {

		register('ادخل البيانات عند التسجيل صحيحه حتي يتم قبول حسابك وتأكد من ادخال المعلومات بالآنجليزيه فقط','blue',$reseller);

}

if(isset($_GET['vcode'])) {

	$vid=$_GET['vcode'];

		ResendCode('A new activation link has been sent to your email address. Please follow the activation link to re-activate your account.','green',$vid);

}

if(isset($_REQUEST['registered'])) {

		    ShowProcceed("شكرآ لك تم التسجيل بنجاح ولكن حسابك قيد التفعيل ولتفعيل حسابك برجاء تفعيل الحساب عبر البريد الآلكتروني او الهاتف.",'green');



}

if(isset($_GET['reset'])) {

		RecoverPassword('','');

}



?>