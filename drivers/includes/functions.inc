<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Language" content="ar-eg">
</head>

<?php defined('SMSPanel') or die('Snooping around is not allowed. Please use the front door');

$version_id = $sconfig['verssion_id'];

/*

File name: 		index.inc

Description:	This is main fuction that does allmost every stuff around here

Developer: 		Chuka Okoye (Mr. White)

Date: 			23/10/2015

*/

set_time_limit(0);

function getUserIP() {

	if (!empty($_SERVER["HTTP_CLIENT_IP"]))

	{

	 $ip = $_SERVER["HTTP_CLIENT_IP"];

	}

	elseif (!empty($_SERVER["HTTP_X_FORWARDED_FOR"]))

	{

	 $ip = $_SERVER["HTTP_X_FORWARDED_FOR"];

	}

	else

	{

	 $ip = $_SERVER["REMOTE_ADDR"];

	}

return $ip;	

}

function isLogedIn() {

	//check if admin

	  if (isset($_COOKIE['verify-SMSPanel-Admin']) && isset($_SESSION['manageUser'])) {

		  return true;

	  } else {

	  	if (!isset($_COOKIE['verify-SMSPanel']) || !isset($_COOKIE['CP-SMSPanel']) || empty($_COOKIE['CP-SMSPanel']))

	  	 { return false; } else { return true; }

	  }

}

function adminLogedIn() {

  if (!isset($_COOKIE['verify-SMSPanel-Admin']) || !isset($_COOKIE['CP-SMSPanel-Admin']) || empty($_COOKIE['CP-SMSPanel-Admin']))

   { return false; } else { return true; }

}

function getUser() {

	if (isset($_COOKIE['verify-SMSPanel-Admin']) && isset($_SESSION['manageUser'])) {

		return $_SESSION['manageUser'];

	} else {

		return @$_COOKIE['CP-SMSPanel']; 

	}

}

function getAdmin() {

return @$_COOKIE['CP-SMSPanel-Admin']; 

}

function showMessage($message, $class) {

	if(empty($class) || is_numeric($class)) {

		$class='blue';	

	}

	echo '<div id="messageBox" style="max-width: 600px;" class="message '.$class.'">'.$message.'<a href="javascript:{}" title="Dismiss" id="close-message" onClick="document.getElementById(\'messageBox\').style.display=\'none\';">x</a></div><br/>';	

	

	echo '<script>';

	echo "Alert.render('".$message."')";

	echo '</script>';		

}

if(!function_exists('showLoginPasswordProtect')) {

	function showLoginPasswordProtect($message, $class) {



	//load defaults 

	$siteName = getSetting('siteName');

	$siteLogo = getSetting('siteLogo');

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;

	if(empty($siteLogo)){ $siteLogo = 'media/images/logo.png'; } else {

		$siteLogo = BASE.'media/uploads/'.$siteLogo;

	}	

?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> تسجيل الدخول</div>

			<div id="login-form">

            <form method="post" action="" name="login">

                <p><input type="text" name="access_login" placeholder=" أسم المستخدم الخاص بك"></p>

                <p><input type="password" name="access_password" placeholder="كلمه المرور الخاص بك"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >تسجيل الدخول الآن</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>index.php?register&p"><button>أنشاء حساب جديد الآن</button></a></p>

            <p><a href="<?php echo BASE; ?>index.php?reset&p" class="link">استرجاع كلمه المرور</a></p>

			<div id="login-loading"><img src="<?php echo BASE; ?>media/images/spin.gif" /> Processing...</div>

        </div>

		<div id="divide"></div>

		<div id="login-logo">

			<img src="<?php echo $siteLogo; ?>" alt="<?php echo $siteName; ?>" title="<?php echo $siteName; ?>" />

		</div>

    </div> 

	

  </body>

</html>	

 <?php 

 	die();

 //end sho login	

	}

}



if(!function_exists('showAdminLoginPasswordProtect')) {

	function showAdminLoginPasswordProtect($message, $class) {



	//load defaults 

	$siteName = getSetting('siteName');

	$siteLogo = getSetting('siteLogo');

	 $siteName = 'SMSPanel v1.0'; 

	 $siteLogo = 'media/images/logo.png'; 

?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> Admin login</div>

			<div id="login-form">

            <form method="post" action="" name="login">

                <p><input type="text" name="access_login" placeholder=" أسم المستخدم الخاص بك"></p>

                <p><input type="password" name="access_password" placeholder="كلمه المرور الخاص بك"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >تسجيل الدخول الآن</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>administrator/index.php?reset&p" class="link">استرجاع كلمه المرور</a></p>

			<div id="login-loading"><img src="<?php echo BASE; ?>media/images/spin.gif" /> Processing...</div>

        </div>

		<div id="divide"></div>

		<div id="login-logo">

			<img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" title="<?php echo $siteName; ?>" />

		</div>

    </div> 

	

  </body>

</html>	

 <?php 

 	die();

 //end sho login	

	}

}





if(!function_exists('ShowProcceed')) {

	function ShowProcceed($message, $class) {

?>

    	<div id="mess" style="width: 100%;">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

		

        <p style="text-align:center;"><a href="<?php echo BASE; ?>index.php"><button class="submit" style="float:none; margin-top: 140px; width: 200px; height: 50px; font-size: 18px;">Continue</button></a></p>

	

  </body>

</html>	

 <?php 

 	die();

	}

}







function setBackground() {

	$query="SELECT * FROM settings WHERE field = 'siteBackground'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$imageURL = $row['value'];	

	if(empty($imageURL)) {

	return false;

	} else {

	return $imageURL;

	}

}

function setGateway($id,$value) {

	$query="SELECT * FROM gateways WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row[$value];	

	if(empty($value)) {

	return '';

	} else {

	return $value;

	}

}

function setBackgroundColor() {

	$query="SELECT * FROM settings WHERE field = 'backgroundColor'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$color = $row['value'];	

	if(empty($color)) {

	return false;

	} else {

	return $color;

	}

}

function setHeaderColor() {

	$query="SELECT * FROM settings WHERE field = 'headerColor'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$color = $row['value'];	

	if(empty($color)) {

	return false;

	} else {

	return $color;

	}

}

function setFooterColor() {

	$query="SELECT * FROM settings WHERE field = 'footerColor'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$color = $row['value'];	

	if(empty($color)) {

	return false;

	} else {

	return $color;

	}

}

function isReseller($id) {

	$query="SELECT * FROM customers WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$isReseller = $row['isReseller'];	

	if($isReseller < 1) {

	return false;

	} else {

	return true;

	}

}



function checkEmailAddress($email) {

    // First, we check that there's one @ symbol, and that the lengths are right

    if (!ereg("^[^@]{1,64}@[^@]{1,255}$", $email)) {

        // Email invalid because wrong number of characters in one section, or wrong number of @ symbols.

        return false;

    }

    // Split it into sections to make life easier

    $email_array = explode("@", $email);

    $local_array = explode(".", $email_array[0]);

    for ($i = 0; $i < sizeof($local_array); $i++) {

         if (!ereg("^(([A-Za-z0-9!#$%&'*+/=?^_`{|}~-][A-Za-z0-9!#$%&'*+/=?^_`{|}~\.-]{0,63})|(\"[^(\\|\")]{0,62}\"))$", $local_array[$i])) {

            return false;

        }

    }    

    if (!ereg("^\[?[0-9\.]+\]?$", $email_array[1])) { // Check if domain is IP. If not, it should be valid domain name

        $domain_array = explode(".", $email_array[1]);

        if (sizeof($domain_array) < 2) {

                return false; // Not enough parts to domain

        }

        for ($i = 0; $i < sizeof($domain_array); $i++) {

            if (!ereg("^(([A-Za-z0-9][A-Za-z0-9-]{0,61}[A-Za-z0-9])|([A-Za-z0-9]+))$", $domain_array[$i])) {

                return false;

            }

        }

    }

    return true;

}



function transactionStatus($id) {

	$query="SELECT * FROM transaction_status WHERE id = '$id'"; 

	$result = mysql_query($query) ; 

	$row = mysql_fetch_assoc($result); 

	$value = $row['value'];	

	return $value;

}



function getName($id) {

	$query="SELECT * FROM customers WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row['name'];	

	return $value;

}

function getAdminName($id) {

	$query="SELECT * FROM users WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row['firstName'].' '.$row['lastName'];	

	return $value;

}

function getUsername($id) {

	$query="SELECT * FROM customers WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row['username'];	

	if($id < 1) {

		$value	= 'Administrator';

	}	

	return $value;

}

function getAdminUsername($id) {

	$query="SELECT * FROM users WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row['username'];	

	return $value;

}

function transactionAmount($id) {

	$query="SELECT * FROM transactions WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row['cost'];	

	return $value;

}

function transactionGateway($id) {

	$query="SELECT * FROM transactions WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$gateway = $row['gateway'];	

	$result = mysql_query("SELECT * FROM paymentgateways WHERE id='$gateway'"); 

	$row = mysql_fetch_assoc($result);

	$value = $row['name'];	

	return $value;

}

function userData($field, $id) {

	$query="SELECT * FROM customers WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row[ $field ];	

	if(empty($value)) {

	return 'not available';

	} else {

	return $value;

	}

}

function adminData($field, $id) {

	$query="SELECT * FROM users WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row[ $field ];	

	if(empty($value)) {

	return 'not available';

	} else {

	return $value;

	}

}

function shorten($intro, $len) { 

	$string = strip_tags($intro);

	if (strlen($string) > $len) 

	{

    // truncate string

    $stringCut = substr($string, 0, $len);

    // make sure it ends in a word so assassinate doesn't become ass...

    $string = substr($stringCut, 0, strrpos($stringCut, ' ')).'... '; 

	}

	

	return $string;

}



//sms functions 

function correctCommas($csv) {

	$inpt= array("\r","\n",' ',';',':','"','.',"'",'`','\t','(',')','<','>','{','}','#',"\r\n",'-','_','?','+');

	$oupt= array(',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',',');

	$csv = str_replace($inpt,$oupt,$csv);

	while(strripos($csv,',,') !== false){

		$csv = str_replace(',,',',',$csv);

	}

	while(strripos($csv,'\r') !== false){

		$csv = str_replace('\r',',',$csv);

	}

	while(strripos($csv,'\n') !== false){

		$csv = str_replace('\n',',',$csv);

	}

	$csv = str_replace($inpt,$oupt,$csv);

	return $csv;

}



function alterPhone($gsm,$code) {

	$array = is_array($gsm);

	$gsm = ($array) ? $gsm : explode(",",$gsm);

	$homeCountry = $code;

	$outArray = array();

	foreach($gsm as $item)

	{

		if(!empty($item)){

			$item1 = (string)$item;

			$q = substr($item1,0,1);

			$w = substr($item1,0,3);

			$item1 = (substr($item1,0,1) == "+") ? substr($item1,1) : $item1;

			$item1 = (substr($item1,0,3) == "009") ? $homeCountry.substr($item1,3): $item1;

			$item1 = (substr($item1,0,1) == "0") ? $homeCountry.substr($item1,1): $item1;

			$item1 = (substr($item1,0,strlen($homeCountry)) == $homeCountry) ? $homeCountry.substr($item1,strlen($homeCountry)): $item1;

			$outArray[] = $item1;

		}

	}

	return ($array) ? $outArray : implode(",",$outArray);

}

function removeDuplicate($myArray) {

	$array = is_array($myArray);

	$myArray = ($array) ? $myArray: explode(",",$myArray);

	$myArray = array_flip(array_flip(array_reverse($myArray,true)));

	return ($array) ? $myArray : implode(',',$myArray);

}

function mceil($number) {

	$array = explode(".",$number);

	$deci = ((int)$array[1] > 0) ? 1 : 0;

	return (int)$array[0] + $deci;

}



function  smsCost($recipientList,$textMessage) {

	//get message lenght 

	$requiredCredit = 0;

	$messageLenght = strlen($textMessage);

	$smsLength = "160";



	//get price based on destination country

	$pricelist =  getSetting('smsCost');

	$searchKeys = array("\r\n","<br>","\n","\r");

	$replaceKeys = array(",",",",",",",");

	$pricelist = str_replace( $searchKeys , $replaceKeys , $pricelist);

	$pricelist = explode(',',$pricelist);

	$countryPrices = array();

	for($i=0; $i<count($pricelist); $i++) { 

		$countryPrices[] = explode('=',$pricelist[$i]);

	}

	

	$priceList = $countryPrices; 

	

	$credit = array();

	$credito = 0;

    $recipientList = explode(",", $recipientList);

	

	for($i=0; $i < count($recipientList); $i++){			

		//get the sms unit to use for each number

		$unitcost=getSetting('defaultCost');

		for($j=0; $j < count($priceList); $j++){

			$len = strlen($priceList[$j][0]); 

							

			if(substr($recipientList[$i],0,$len) == $priceList[$j][0] && $len > 0 && !empty($priceList[$j][0])){

				$unitcost=$priceList[$j][1]; 

				break;

			}

		} 

	

						

		$requiredCredit2 = ($messageLenght <= $smsLength) ? mceil($messageLenght/$smsLength)*$unitcost : mceil($messageLenght/$smsLength)*$unitcost ;   

		$requiredCredit = $requiredCredit + $requiredCredit2;

	} //end for si

	return $requiredCredit;

}



function  smsUnicodeCost($recipientList,$textMessage) {

	//get message lenght 

	$requiredCredit = 0;

	$messageLenght = strlen($textMessage);

	$smsLength = "70";



	//get price based on destination country

	$pricelist =  getSetting('smsCost');

	$searchKeys = array("\r\n","<br>","\n","\r");

	$replaceKeys = array(",",",",",",",");

	$pricelist = str_replace( $searchKeys , $replaceKeys , $pricelist);

	$pricelist = explode(',',$pricelist);

	$countryPrices = array();

	for($i=0; $i<count($pricelist); $i++) { 

		$countryPrices[] = explode('=',$pricelist[$i]);

	}

	

	$priceList = $countryPrices; 

	

	$credit = array();

	$credito = 0;

    $recipientList = explode(",", $recipientList);

	

	for($i=0; $i < count($recipientList); $i++){			

		//get the sms unit to use for each number

		$unitcost=getSetting('defaultCost');

		for($j=0; $j < count($priceList); $j++){

			$len = strlen($priceList[$j][0]); 

							

			if(substr($recipientList[$i],0,$len) == $priceList[$j][0] && $len > 0 && !empty($priceList[$j][0])){

				$unitcost=$priceList[$j][1]; 

				break;

			}

		} 

	

						

		$requiredCredit2 = ($messageLenght <= $smsLength) ? mceil($messageLenght/$smsLength)*$unitcost : mceil($messageLenght/$smsLength)*$unitcost ;   

		$requiredCredit = $requiredCredit + $requiredCredit2;

	} //end for si

	return $requiredCredit;

}



function countMessageRecipient($id) {

	$sql = "SELECT sum((LENGTH(recipient)+1) - LENGTH(REPLACE(recipient, ',', ''))) as value1 FROM sentmessages WHERE id = '$id'";

	$result = mysql_query($sql) or die(mysql_error());

	$row = mysql_fetch_assoc($result);

	return $num = round($row['value1']);		

}



function sendMessage($senderID,$recipientList,$textMessage,$customer,$sentFrom,$IP) {

	

	//get gateway settings 

		$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );

		$recipientList = implode(',',array_unique(explode(',', $recipientList)));

		$senderID2 = $senderID;

		$textMessage2 = $textMessage;

		$textMessage = urlencode($textMessage);

		$senderID = urlencode($senderID);

		$activeGateway = getSetting('activeGateway');	

		$sendURL = setGateway($activeGateway,'sendAPI');

		$sendURL = str_replace('[SENDER]', $senderID, $sendURL);

		$sendURL = str_replace('[TO]', $recipientList, $sendURL);

		$sendURL = str_replace('[MESSAGE]', $textMessage, $sendURL);

		$batchSize = setGateway($activeGateway,'batchSize');

		$successWord = setGateway($activeGateway,'successWord');	

        $url = $sendURL;




	//open connection

	$ch = curl_init();

	

	//set the url, number of POST vars, POST data

		curl_setopt($ch,CURLOPT_URL,$url);
		
		curl_setopt($ch,CURLOPT_HTTPHEADER,array("Expect:"));

        curl_setopt($ch,CURLOPT_RETURNTRANSFER, true);

	

	//execute post

	$response = curl_exec($ch);



	if(!$response) {

	  $return = 'Connection to Gateway Failed. Please try again later';  //. curl_error($ch)

	 $now = date("Y-m-d H:i:s");

	 $messageLenght = strlen($textMessage);

	 $messageLenght = mceil($messageLenght/160);

	 $unitsUsed = smsCost($recipientList,$textMessage);

	 

	$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

	VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Gateway Failed');"); 

	  } else {

           $return = 'Message Sending Failed. Unknown Gateway Error';

              if((stripos($response,$successWord) !== false)) {

                   $return = 'Message Sent Successfully';

				   //get units used

				   $unitsUsed = smsCost($recipientList,$textMessage);

				   //get new balance

				   $smsBalance = userData('smsBalance',$customer);

				   $newBalance = $smsBalance - $unitsUsed;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/160);

				   //update balance

				   mysql_query("UPDATE `customers` SET `smsBalance` = '$newBalance' WHERE `id` = '$customer'");

				   //add to sent items

				   $now = date("Y-m-d H:i:s");

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Sent', '$unitsUsed', '$sentFrom', '$IP', 'Successfully Sent');");

               } else {

                   $return = 'Message sending was interrupted. Please report this error to your admin if it persists ERROR: '.$response;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/160);

				   $unitsUsed = smsCost($recipientList,$textMessage);

				   //add to sent items as failed

				   $now = date("Y-m-d H:i:s");

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Sending Interrupted');");

               }

	  }  

	  

	return $return; 

	curl_close($ch);	

}


function utf8_to_unicode($str) {
	$unicode = array();
	$values = array();
	$lookingFor = 1;

	for ($i = 0; $i < strlen($str); $i++) {	
		$thisValue = ord($str[$i]);
		
		if ($thisValue < 128)			
			$unicode[] = str_pad(dechex($thisValue), 4, "0", STR_PAD_LEFT);
		else {
			if (count($values) == 0) $lookingFor = ($thisValue < 224) ? 2 : 3;
				$values[] = $thisValue;
			if (count($values) == $lookingFor) {
				$number = ($lookingFor == 3) ?
				(($values[0] % 16) * 4096) + (($values[1] % 64) * 64) + ($values[2] % 64):
				(($values[0] % 32) * 64) + ($values[1] % 64);
				$number = strtoupper(dechex($number));		
				$unicode[] = str_pad($number, 4, "0", STR_PAD_LEFT);
				$values = array();
				$lookingFor = 1;
			} 
		} 
	} return ($unicode);
}

function sendUnicodeMessage($senderID,$recipientList,$textMessage,$customer,$sentFrom,$IP) {

	

	//get gateway settings 

		$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );

		$recipientList = implode(',',array_unique(explode(',', $recipientList)));

		$senderID2 = $senderID;

		$textMessage2 = $textMessage;

		

		//convert to UNICODE if required

		if( getSetting('localEncode') > 0) {

			$textMessage2 = utf8_to_unicode($textMessage2);
			$textMessage2 = implode("", $textMessage2);
		}

		

		//$textMessage = urlencode($textMessage);

		$senderID = urlencode($senderID);

		$activeGateway = getSetting('activeGateway');	

		$sendURL = setGateway($activeGateway,'unicodeAPI');

		$sendURL = str_replace('[SENDER]', $senderID, $sendURL);

		$sendURL = str_replace('[TO]', $recipientList, $sendURL);

		$sendURL = str_replace('[MESSAGE]', $textMessage2, $sendURL);

		$batchSize = setGateway($activeGateway,'batchSize');

		$successWord = setGateway($activeGateway,'successWord');	

        $url = $sendURL;




	//open connection

	$ch = curl_init();

	

	//set the url, number of POST vars, POST data

		curl_setopt($ch,CURLOPT_URL,$url);
		
		curl_setopt($ch,CURLOPT_HTTPHEADER,array("Expect:"));

        curl_setopt($ch,CURLOPT_RETURNTRANSFER, true);

	

	//execute post

	$response = curl_exec($ch);



	if(!$response) {

	  $return = 'Connection to Gateway Failed. Please try again later';  //. curl_error($ch)

	 $now = date("Y-m-d H:i:s");

	 $messageLenght = strlen($textMessage);

	 $messageLenght = mceil($messageLenght/70);

	 $unitsUsed = smsUnicodeCost($recipientList,$textMessage);

	 

	$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

	VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Gateway Failed');"); 

	  } else {

           $return = 'Message Sending Failed. Unknown Gateway Error';

              if((stripos($response,$successWord) !== false)) {

                   $return = 'Message Sent Successfully';

				   //get units used

				   $unitsUsed = smsCost($recipientList,$textMessage);

				   //get new balance

				   $smsBalance = userData('smsBalance',$customer);

				   $newBalance = $smsBalance - $unitsUsed;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/70);

				   //update balance

				   mysql_query("UPDATE `customers` SET `smsBalance` = '$newBalance' WHERE `id` = '$customer'");

				   //add to sent items

				   $now = date("Y-m-d H:i:s");

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Sent', '$unitsUsed', '$sentFrom', '$IP', 'Successfully Sent');");

               } else {

                   $return = 'Message sending was interrupted. Please report this error to your admin if it persists ERROR: '.$response;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/70);

				   $unitsUsed = smsCost($recipientList,$textMessage);

				   //add to sent items as failed

				   $now = date("Y-m-d H:i:s");

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Sending Interrupted');");

               }

	  }  

	  

	return $return; 

	curl_close($ch);	

}





//admin function to send sms

function sendAdminMessage($senderID,$recipientList,$textMessage,$customer,$sentFrom,$IP) {

	$customer='0';

	//get gateway settings 

		$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );

        $recipientList = implode(',',array_unique(explode(',', $recipientList)));

		$senderID2 = $senderID;

        $textMessage2 = $textMessage;

		$textMessage = urlencode($textMessage);

		$senderID = urlencode($senderID);

		$activeGateway = getSetting('activeGateway');	

		$sendURL = setGateway($activeGateway,'sendAPI');

		$sendURL = str_replace('[SENDER]', $senderID, $sendURL);

		$sendURL = str_replace('[TO]', $recipientList, $sendURL);

		$sendURL = str_replace('[MESSAGE]', $textMessage, $sendURL);

			

		$batchSize = setGateway($activeGateway,'batchSize');

		$successWord = setGateway($activeGateway,'successWord');	

        $url = $sendURL;



	//open connection

	$ch = curl_init();

	

	//set the url, number of POST vars, POST data

		curl_setopt($ch,CURLOPT_URL,$url);

		curl_setopt($ch,CURLOPT_HTTPHEADER,array("Expect:"));

        curl_setopt($ch,CURLOPT_RETURNTRANSFER, true);

	

	//execute post

	$response = curl_exec($ch);



	if(!$response) {

	  $return = 'Connection to Gateway Failed. Please try again later';  //. curl_error($ch)

	 $now = date("Y-m-d H:i:s");

	 $messageLenght = strlen($textMessage);

	 $messageLenght = mceil($messageLenght/160);

	 $customer = 0;

	$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

	VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', 'Admin', '$IP', 'Gateway Failed');"); 

	  } else {

           $return = 'Message Sending Failed. Unknown Gateway Error';

              if((stripos($response,$successWord) !== false)) {

                   $return = 'Message Sent Successfully';

				   //get units used

				   $unitsUsed = smsCost($recipientList,$textMessage);

				   //get new balance

				   $smsBalance = userData('smsBalance',$customer);

				   $newBalance = $smsBalance - $unitsUsed;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/160);

				   //no need to update balance

				   

				   //add to sent items

				   $customer = 0;

				   $now = date("Y-m-d H:i:s");

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Sent', '$unitsUsed', 'Admin', '$IP', 'Successfully Sent');");

               } else {

                   $return = 'Message sending was interrupted. Please report this error to your admin if it persists ERROR: '.$response;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/160);

				   

				   //add to sent items as failed

				   $now = date("Y-m-d H:i:s");

				   $customer = 0;

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Sending Interrupted');");

               }

	  }  

	  

	return $return; 

	curl_close($ch);	

}



function sendAdminUnicodeMessage($senderID,$recipientList,$textMessage,$customer,$sentFrom,$IP) {

	$customer='0';

	//get gateway settings 

		$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );

        $recipientList = implode(',',array_unique(explode(',', $recipientList)));

		$senderID2 = $senderID;

        $textMessage2 = $textMessage;

		

		//convert to UNICODE if required

		if( getSetting('localEncode') > 0) {

			$textMessage2 = utf8_to_unicode($textMessage2);
			$textMessage2 = implode("", $textMessage2);
		}

		
		//$textMessage = urlencode($textMessage);
		$senderID = urlencode($senderID);
		
		$activeGateway = getSetting('activeGateway');	

		$sendURL = setGateway($activeGateway,'unicodeAPI');

		$sendURL = str_replace('[SENDER]', $senderID, $sendURL);

		$sendURL = str_replace('[TO]', $recipientList, $sendURL);

		$sendURL = str_replace('[MESSAGE]', $textMessage2, $sendURL);


		$batchSize = setGateway($activeGateway,'batchSize');

		$successWord = setGateway($activeGateway,'successWord');	

        $url = $sendURL;




	//open connection

	$ch = curl_init();

	

	//set the url, number of POST vars, POST data

		curl_setopt($ch,CURLOPT_URL,$url);

		curl_setopt($ch,CURLOPT_HTTPHEADER,array("Expect:"));

        curl_setopt($ch,CURLOPT_RETURNTRANSFER, true);

	

	//execute post

	$response = curl_exec($ch);



	if(!$response) {

	  $return = 'Connection to Gateway Failed. Please try again later';  //. curl_error($ch)

	 $now = date("Y-m-d H:i:s");

	 $messageLenght = strlen($textMessage);

	 $messageLenght = mceil($messageLenght/70);

	 $customer = 0;

	$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

	VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', 'Admin', '$IP', 'Gateway Failed');"); 

	  } else {

           $return = 'Message Sending Failed. Unknown Gateway Error';

              if((stripos($response,$successWord) !== false)) {

                   $return = 'Message Sent Successfully';

				   //get units used

				   $unitsUsed = smsUnicodeCost($recipientList,$textMessage);

				   //get new balance

				   $smsBalance = userData('smsBalance',$customer);

				   $newBalance = $smsBalance - $unitsUsed;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/70);

				   //no need to update balance

				   

				   //add to sent items

				   $customer = 0;

				   $now = date("Y-m-d H:i:s");

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Sent', '$unitsUsed', 'Admin', '$IP', 'Successfully Sent');");

               } else {

                   $return = 'Message sending was interrupted. Please report this error to your admin if it persists ERROR: '.$response;

				   $messageLenght = strlen($textMessage);

				   $messageLenght = mceil($messageLenght/70);

				   

				   //add to sent items as failed

				   $now = date("Y-m-d H:i:s");

				   $customer = 0;

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

			VALUES (NULL, '$textMessage2', '$senderID2', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Sending Interrupted');");

               }

	  }  

	  

	return $return; 

	curl_close($ch);	

}



function sendScheduleMessage($id,$senderID,$recipientList,$textMessage,$customer,$IP, $Unicode) {

	$error = 0;

	$now = date("Y-m-d H:i:s");

			mysql_query("UPDATE `scheduledmessages` SET 

			`status` = 'Sending'

			 WHERE `id` = '$id'");

			 

			$userID = $customer;

			$sentFrom = 'Web SMS';



			//check credit stuffs if its not an admin 

			if($customer > 0) {

				//check user's SMS balance

				$smsBalance = userData('smsBalance',$userID);	

			//check required SMS units for each recipients

			$requiredCredit = smsCost($recipientList,$textMessage);

			if($Unicode > 0) {

				$requiredCredit = smsUnicodeCost($recipientList,$textMessage);

			}

			

				if(($smsBalance < 0) || ($smsBalance < $requiredCredit)) {

					//insert message in sent items

					$messageLenght = strlen($textMessage);

					$messageLenght = mceil($messageLenght/160); //do another check here.

					$now = date("Y-m-d H:i:s");

					$extraUnit = $requiredCredit - $smsBalance;

					$status = 'Expired';

					$error = 'Insufficient Balance';

					$error = 1;	

					//update status

					mysql_query("UPDATE `scheduledmessages` SET 

					`status` = '$status'

					WHERE `id` = '$id'");

			 

					$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`) 

					VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Insufficient Balance');");	

				}

			}

			

			//send massage in batches

			$activeGateway = getSetting('activeGateway');	

			$batchSize = setGateway($activeGateway,'batchSize');

			$recipientcount = explode(',',$recipientList);

			

		if($error < 1) {	

			if(count($recipientcount) > $batchSize){			

				for($q=0; $q < count($recipientcount); $q+=$batchSize){

					$tempRecipientsList = array();

					for($w=0; $w<$batchSize; $w++){

						if(!empty($recipientcount[$q+$w])) {

							$tempRecipientsList[] = $recipientcount[$q+$w];

						}

					}

					$recipientList2 = implode(',',$tempRecipientsList);

					if($Unicode > 0) {

						$sendingResponse = sendUnicodeMessage($senderID,$recipientList2,$textMessage,$customer,$sentFrom,$IP);

					} else {

						$sendingResponse = sendMessage($senderID,$recipientList2,$textMessage,$customer,$sentFrom,$IP);	

					}

				}	

			} 

			else

			{ 

				if($Unicode > 0) {

					$sendingResponse = sendUnicodeMessage($senderID,$recipientList2,$textMessage,$customer,$sentFrom,$IP);	

				} else {	

					$sendingResponse = sendMessage($senderID,$recipientList,$textMessage,$customer,$sentFrom,$IP);

				}

			}

			$status = 'Expired';

			if(strpos($sendingResponse,'Message Sent Successfully') !== false) {

				$status = 'Completed';

				$error = 'Successfully Sent';

			} else {

				$status = 'Expired';

				$error = 'Sending Interrupted';

			}	

			//update status

			mysql_query("UPDATE `scheduledmessages` SET 

			`status` = '$status'

			 WHERE `id` = '$id'");

		}

}



function sendEmail($from,$sender,$subject,$to,$message) {

	$email_from = $sender.'<'.$from.'>';
	$email_subject = $subject;
	$to = $to;

	//Build HTML Message

	$emessage = '<html><body>';
	$emessage .= $message;
	$emessage .= '</body></html>';

	//Send Mail
	$headers = "From: " . $email_from . "\r\n";
	$headers .= 'MIME-Version: 1.0' . "\r\n";
	$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";	
	$headers .= 'X-Mailer: PHP/' . phpversion();
	//Send the email!

	mail($to,$email_subject,$emessage,$headers);	

}

class Currency {

	function Symbul($id) {

		$result = mysql_query("SELECT * FROM customers WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$client_currency = $row['currency'];



		$result = mysql_query("SELECT * FROM currencies WHERE id = '$client_currency'"); 

		$row = mysql_fetch_assoc($result);

		$currency_name = $row['name'];

		$currency_symbul = $row['symbul'];

		$name = $currency_symbul;

		

	return $name;

	}



	function Rate($id) {

		$result = mysql_query("SELECT * FROM customers WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$client_currency = $row['currency'];



		$result = mysql_query("SELECT * FROM currencies WHERE id = '$client_currency'"); 

		$row = mysql_fetch_assoc($result);

		$currency_rate = $row['rate'];

		$name = $currency_rate;



	return $name;

	}

}

class DefaultCurrency {

	function Symbul($id) {

		$result = mysql_query("SELECT * FROM settings WHERE field = 'defaultCurrency'"); 

		$row = mysql_fetch_assoc($result);

		$client_currency = $row['value'];



		$result = mysql_query("SELECT * FROM currencies WHERE id = '$client_currency'"); 

		$row = mysql_fetch_assoc($result);

		$currency_name = $row['name'];

		$currency_symbul = $row['symbul'];

		$name = $currency_symbul;

		

	return $name;

	}



	function Rate($id) {

		$result = mysql_query("SELECT * FROM settings WHERE field = 'defaultCurrency'"); 

		$row = mysql_fetch_assoc($result);

		$client_currency = $row['value'];



		$result = mysql_query("SELECT * FROM currencies WHERE id = '$client_currency'"); 

		$row = mysql_fetch_assoc($result);

		$currency_rate = $row['rate'];

		$name = $currency_rate;



	return $name;

	}

}



function isCustomGateway($id) {

		$result = mysql_query("SELECT * FROM paymentgateways WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$custom = $row['custom'];

	if($custom >0) {

	return true;

	} else {

		return false;

	}

}



function gatewayData($id, $data) {

$result = mysql_query("SELECT * FROM paymentgateways WHERE id='$id'"); 

$row = mysql_fetch_assoc($result);

return $row[$data];	

}



function gatewayName($id) {

		$result = mysql_query("SELECT * FROM paymentgateways WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$name = $row['name'];

	if($id < 1) {

		$name	= 'Admin Transfer';

	}		

		return $name;

}



function gatewayAlias($id) {

		$result = mysql_query("SELECT * FROM paymentgateways WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$name = $row['alias'];

	if($id < 1) {

		$name	= 'System';

	}		

		return $name;

}



function countContacts($id) {

	$sql = "SELECT * FROM contacts WHERE phonebook = '$id'";

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

return $num;

}



function isAdminUser($id) {

	$sql = "SELECT * FROM users WHERE customer = '$id'";

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

	if($num >0) {

		return true;

	} else {

		return false;	

	}

}

function customerType($id) {

	$sql = "SELECT * FROM customers WHERE id = '$id'";

	$result = mysql_query($sql);

	$row = mysql_fetch_assoc($result);

	$type = $row['isReseller'];

	if($type >0) {

		return 'Reseller';

	} else {

		return 'Consumer';	

	}

}

function customerStatus($id) {

	$sql = "SELECT * FROM customers WHERE id = '$id'";

	$result = mysql_query($sql);

	$row = mysql_fetch_assoc($result);

	$type = $row['suspended'];

	if($type >0) {

		return 'Suspended';

	} else {

		return 'Active';	

	}

}



function countCustomers($type) {

	switch($type) {

		case 'active':

		$sql = "SELECT * FROM customers WHERE suspended = '0'";

		break;

		case 'suspended':

		$sql = "SELECT * FROM customers WHERE suspended = '1'";

		break;

		case 'all':

		$sql = "SELECT * FROM customers";

		break;				

	}

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

return $num;

}

function getCountryList($select) {

			if(!empty($select)) {echo '<option value="'.$select.'">'.$select.'</option>'; }

			echo '			<option value="CY">Cyprus</option>

							<option value="Afghanistan">Afghanistan</option>

							<option value="Albania">Albania</option>

							<option value="Algeria">Algeria</option>

							<option value="Andorra">Andorra</option>

							<option value="Angola">Angola</option>

							<option value="Antigua and Barbuda">Antigua and Barbuda</option>

							<option value="Argentina">Argentina</option>

							<option value="Armenia">Armenia</option>

							<option value="Australia">Australia</option>

							<option value="Austria">Austria</option>

							<option value="Azerbaijan">Azerbaijan</option>

							<option value="Bahamas">Bahamas</option>

							<option value="Bahrain">Bahrain</option>

							<option value="Bangladesh">Bangladesh</option>

							<option value="Barbados">Barbados</option>

							<option value="Belarus">Belarus</option>

							<option value="Belgium">Belgium</option>

							<option value="Belize">Belize</option>

							<option value="Benin">Benin</option>

							<option value="Bhutan">Bhutan</option>

							<option value="Bolivia">Bolivia</option>

							<option value="Bosnia and Herzegovina">Bosnia and Herzegovina</option>

							<option value="Botswana">Botswana</option>

							<option value="Brazil">Brazil</option>

							<option value="Brunei">Brunei</option>

							<option value="Bulgaria">Bulgaria</option>

							<option value="Burkina Faso">Burkina Faso</option>

							<option value="Burundi">Burundi</option>

							<option value="Cambodia">Cambodia</option>

							<option value="Cameroon">Cameroon</option>

							<option value="Canada">Canada</option>

							<option value="Cape Verde">Cape Verde</option>

							<option value="Central African Republic">Central African Republic</option>

							<option value="Chad">Chad</option>

							<option value="Chile">Chile</option>

							<option value="China">China</option>

							<option value="Colombia">Colombia</option>

							<option value="Comoros">Comoros</option>

							<option value="Congo">Congo</option>

							<option value="Costa Rica">Costa Rica</option>

							<option value="C&Atilde;´te dâ€™Ivoire">C&Atilde;´te dâ€™Ivoire</option>

							<option value="Croatia">Croatia</option>

							<option value="Cyprus">Cuba</option>

							<option value="CY">Cyprus</option>

							<option value="Czech Republic">Czech Republic</option>

							<option value="Republic of the Congo<">Democratic Republic of the Congo</option>

							<option value="Denmark">Denmark</option>

							<option value="Djibouti">Djibouti</option>

							<option value="Dominica">Dominica</option>

							<option value="Dominican Republic">Dominican Republic</option>

							<option value="Ecuador">Ecuador</option>

							<option value="Egypt">Egypt</option>

							<option value="El Salvador">El Salvador</option>

							<option value="Equatorial Guinea">Equatorial Guinea</option>

							<option value="Eritrea">Eritrea</option>

							<option value="Estonia">Estonia</option>

							<option value="Ethiopia">Ethiopia</option>

							<option value="Fiji">Fiji</option>

							<option value="Finland">Finland</option>

							<option value="France">France</option>

							<option value="Gabon">Gabon</option>

							<option value="Gambia">Gambia</option>

							<option value="Georgia">Georgia</option>

							<option value="Germany">Germany</option>

							<option value="Ghana">Ghana</option>

							<option value="Greece">Greece</option>

							<option value="Grenada">Grenada</option>

							<option value="Guatemala">Guatemala</option>

							<option value="Guinea">Guinea</option>

							<option value="Guinea-Bissau">Guinea-Bissau</option>

							<option value="Guyana">Guyana</option>

							<option value="Haiti">Haiti</option>

							<option value="Honduras">Honduras</option>

							<option value="Hong Kong S.A.R., China">Hong Kong S.A.R., China</option>

							<option value="Hungary">Hungary</option>

							<option value="Iceland">Iceland</option>

							<option value="India">India</option>

							<option value="Indonesia">Indonesia</option>

							<option value="Iran">Iran</option>

							<option value="Iraq">Iraq</option>

							<option value="Ireland">Ireland</option>

							<option value="Israel">Israel</option>

							<option value="Italy">Italy</option>

							<option value="Jamaica">Jamaica</option>

							<option value="Japan">Japan</option>

							<option value="Jordan">Jordan</option>

							<option value="Kazakhstan">Kazakhstan</option>

							<option value="Kenya">Kenya</option>

							<option value="Kiribati">Kiribati</option>

							<option value="Kosovo">Kosovo</option>

							<option value="Kuwait">Kuwait</option>

							<option value="Kyrgyzstan">Kyrgyzstan</option>

							<option value="Laos">Laos</option>

							<option value="Latvia">Latvia</option>

							<option value="Lebanon">Lebanon</option>

							<option value="Lesotho">Lesotho</option>

							<option value="Liberia">Liberia</option>

							<option value="Libya">Libya</option>

							<option value="Liechtenstein">Liechtenstein</option>

							<option value="Lithuania">Lithuania</option>

							<option value="Luxembourg">Luxembourg</option>

							<option value="Macedonia">Macedonia</option>

							<option value="Madagascar">Madagascar</option>

							<option value="Malawi">Malawi</option>

							<option value="Malaysia">Malaysia</option>

							<option value="Maldives">Maldives</option>

							<option value="Mali">Mali</option>

							<option value="Malta">Malta</option>

							<option value="Marshall Islands">Marshall Islands</option>

							<option value="Mauritania">Mauritania</option>

							<option value="Mauritius">Mauritius</option>

							<option value="Mexico">Mexico</option>

							<option value="Micronesia">Micronesia</option>

							<option value="Moldova">Moldova</option>

							<option value="Monaco">Monaco</option>

							<option value="Mongolia">Mongolia</option>

							<option value="Morocco">Morocco</option>

							<option value="Mozambique">Mozambique</option>

							<option value="Myanmar">Myanmar</option>

							<option value="Namibia">Namibia</option>

							<option value="Nauru">Nauru</option>

							<option value="Nepal">Nepal</option>

							<option value="Netherlands">Netherlands</option>

							<option value="New Zealand">New Zealand</option>

							<option value="Nicaragua">Nicaragua</option>

							<option value="Niger">Niger</option>

							<option value="Nigeria">Nigeria</option>

							<option value="North Korea">North Korea</option>

							<option value="Norway">Norway</option>

							<option value="Oman">Oman</option>

							<option value="Pakistan">Pakistan</option>

							<option value="Palau">Palau</option>

							<option value="Panama">Panama</option>

							<option value="Papua New Guinea">Papua New Guinea</option>

							<option value="Paraguay">Paraguay</option>

							<option value="Peru">Peru</option>

							<option value="Philippines">Philippines</option>

							<option value="Poland">Poland</option>

							<option value="Portugal">Portugal</option>

							<option value="Qatar">Qatar</option>

							<option value="Romania">Romania</option>

							<option value="Russia">Russia</option>

							<option value="Rwanda">Rwanda</option>

							<option value="Saint Kitts and Nevis">Saint Kitts and Nevis</option>

							<option value="Saint Lucia">Saint Lucia</option>

							<option value="Saint Vincent and the Grenadines">Saint Vincent and the Grenadines</option>

							<option value="Samoa">Samoa</option>

							<option value="San Marino">San Marino</option>

							<option value="Sao Tome and Principe">Sao Tome and Principe</option>

							<option value="Saudi Arabia">Saudi Arabia</option>

							<option value="Senegal">Senegal</option>

							<option value="Serbia">Serbia</option>

							<option value="Seychelles">Seychelles</option>

							<option value="Sierra Leone">Sierra Leone</option>

							<option value="Singapore">Singapore</option>

							<option value="Slovakia">Slovakia</option>

							<option value="Slovenia">Slovenia</option>

							<option value="Solomon Islands">Solomon Islands</option>

							<option value="Somalia">Somalia</option>

							<option value="South Africa">South Africa</option>

							<option value="South Korea">South Korea</option>

							<option value="Spain">Spain</option>

							<option value="Sri Lanka">Sri Lanka</option>

							<option value="Sudan">Sudan</option>

							<option value="Suriname">Suriname</option>

							<option value="Swaziland">Swaziland</option>

							<option value="Sweden">Sweden</option>

							<option value="Switzerland">Switzerland</option>

							<option value="Syria">Syria</option>

							<option value="Taiwan">Taiwan</option>

							<option value="Tajikistan">Tajikistan</option>

							<option value="Tanzania">Tanzania</option>

							<option value="Thailand">Thailand</option>

							<option value="Timor-Leste">Timor-Leste</option>

							<option value="Togo">Togo</option>

							<option value="Tonga">Tonga</option>

							<option value="Trinidad and Tobago">Trinidad and Tobago</option>

							<option value="Tunisia">Tunisia</option>

							<option value="Turkey">Turkey</option>

							<option value="Turkmenistan">Turkmenistan</option>

							<option value="Tuvalu">Tuvalu</option>

							<option value="Uganda">Uganda</option>

							<option value="Ukraine">Ukraine</option>

							<option value="United Arab Emirates">United Arab Emirates</option>

							<option value="United Kingdom">United Kingdom</option>

							<option value="United States">United States</option>

							<option value="Uruguay">Uruguay</option>

							<option value="Uzbekistan">Uzbekistan</option>

							<option value="Vanuatu">Vanuatu</option>

							<option value="Vatican">Vatican</option>

							<option value="Venezuela">Venezuela</option>

							<option value="Vietnam">Vietnam</option>

							<option value="Western Sahara">Western Sahara</option>

							<option value="Yemen">Yemen</option>

							<option value="Zambia">Zambia</option>

							<option value="Zimbabwe">Zimbabwe</option>

						</select>';

}



function usernameExist($name) {

	$sql = "SELECT * FROM customers WHERE username = '$name'";

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

	if($num >0) {

		return true;

	} else {

		return false;	

	}

}

function emailExist($name) {

	$sql = "SELECT * FROM customers WHERE email = '$name'";

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

	if($num >0) {

		return true;

	} else {

		return false;	

	}

}



function transactionOwner($id) {

		$result = mysql_query("SELECT * FROM transactions WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$customer = $row['customer'];	

	return $customer;	

}



function processTransaction($id,$gateway) {

		$result = mysql_query("SELECT * FROM transactions WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$status = $row['status'];	

		

		if($status == 3) {

				

		} else {



	//Update transaction record

		mysql_query("UPDATE `transactions` SET 

		`status` =  '3',

		`approvedBy` =  '$gateway'

		WHERE  `id` = '$id';");	

	

		$result = mysql_query("SELECT * FROM transactions WHERE id = '$id'"); 

		$row = mysql_fetch_assoc($result);

		$customer = $row['customer'];

		$units = $row['units'];

		$cost = $row['cost'];	



		$result = mysql_query("SELECT * FROM customers WHERE id = '$customer'"); 

		$row = mysql_fetch_assoc($result);

		$smsBalance = $row['smsBalance'];

		$smsBorrowed = $row['smsBorrowed'];

		$smsPurchase = $row['smsPurchase'];	

		$name = $row['name'];

		$username = $row['username'];

		$email = $row['email'];

		$phone = $row['phone'];	

		

	$Currency = new Currency();

	$userRate = $Currency->Rate($customer);

	$userSymbul = $Currency->Symbul($customer);

		

		$amount = $userSymbul.round($cost*$userRate,2);	

		$newBalance = $units+$smsBalance-$smsBorrowed;

		$newPurchase = $smsPurchase+$units;



		if($smsBorrowed > 0) {

			if($smsBorrowed < $units) {

				$newBorrowed = 0.00;

			} else {

				$newBorrowed = $smsBorrowed-$units;

			}

		} else {

			$newBorrowed = '0.00';	

		}

		

		mysql_query("UPDATE `customers` SET 

		`smsBalance` =  '$newBalance',

		`smsPurchase` =  '$newPurchase',

		`smsBorrowed` =  '$newBorrowed'

		WHERE  `id` = '$customer';");				



	$orderApproveSME = getSetting('orderApproveSME');

	$orderApproveEmail = getSetting('orderApproveEmail');

	$orderApproveEmailSubject = getSetting('orderApproveEmailSubject');

	$smsSender = getSetting('smsSender');

	$emailSender = getSetting('emailSender');

	$emailFrom = getSetting('companyEmail');



		if(!empty($orderApproveSMS)) {

			$mail = str_replace('[USERNAME]', $username, $orderApproveSMS);	

			$mail = str_replace('[CUSTOMER NAME]', $name, $orderApproveSMS);

			$mail = str_replace('[AMOUNT]', $amount, $orderApproveSMS);

			$mail = str_replace('[BALANCE]', $newBalance, $orderApproveSMS);

			$mail = str_replace('[UNITS]', $units, $orderApproveSMS);	

			$mail = strtr ($orderApproveSMS, array ('[UNITS]' => $units,'[BALANCE]' => $newBalance,'[AMOUNT]' => $amount,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));

			$response1 = sendAdminMessage($smsSender,$phone,$mail,'0','Admin',$IP);		

		}



		if(!empty($orderApproveEmail)) {

			$mail = str_replace('[USERNAME]', $username, $orderApproveEmail);	

			$mail = str_replace('[CUSTOMER NAME]', $name, $orderApproveEmail);

			$mail = str_replace('[AMOUNT]', $amount, $orderApproveEmail);

			$mail = str_replace('[BALANCE]', $newBalance, $orderApproveEmail);

			$mail = str_replace('[UNITS]', $units, $orderApproveEmail);

			$mail = strtr ($orderApproveEmail, array ('[UNITS]' => $units,'[BALANCE]' => $newBalance,'[AMOUNT]' => $amount,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));													

			sendEmail($emailFrom,$emailSender,$orderApproveEmailSubject,$email,$mail);

	$date = date('Y-m-d H:i:s');

	$add = mysql_query("INSERT INTO tickets (`id`, `customer`, `subject`, `message`, `date`, `status`, `adminstatus`) 

			VALUES (NULL, '$customer', '$orderApproveEmailSubject', '$mail', '$date', '0', '1');") or die (mysql_error());			

		}						

	}

}



function getCryptedPassword($plaintext, $salt = '', $encryption = 'md5-hex', $show_encrypt = false)

	{

		// Get the salt to use.

		$salt = getSalt($encryption, $salt, $plaintext);



		// Encrypt the password.

		switch ($encryption)

		{

			case 'plain':

				return $plaintext;



			case 'sha':

				$encrypted = base64_encode(mhash(MHASH_SHA1, $plaintext));

				return ($show_encrypt) ? '{SHA}' . $encrypted : $encrypted;



			case 'crypt':

			case 'crypt-des':

			case 'crypt-md5':

			case 'crypt-blowfish':

				return ($show_encrypt ? '{crypt}' : '') . crypt($plaintext, $salt);



			case 'md5-base64':

				$encrypted = base64_encode(mhash(MHASH_MD5, $plaintext));

				return ($show_encrypt) ? '{MD5}' . $encrypted : $encrypted;



			case 'ssha':

				$encrypted = base64_encode(mhash(MHASH_SHA1, $plaintext . $salt) . $salt);

				return ($show_encrypt) ? '{SSHA}' . $encrypted : $encrypted;



			case 'smd5':

				$encrypted = base64_encode(mhash(MHASH_MD5, $plaintext . $salt) . $salt);

				return ($show_encrypt) ? '{SMD5}' . $encrypted : $encrypted;



			case 'aprmd5':

				$length = strlen($plaintext);

				$context = $plaintext . '$apr1$' . $salt;

				$binary = _bin(md5($plaintext . $salt . $plaintext));



				for ($i = $length; $i > 0; $i -= 16)

				{

					$context .= substr($binary, 0, ($i > 16 ? 16 : $i));

				}

				for ($i = $length; $i > 0; $i >>= 1)

				{

					$context .= ($i & 1) ? chr(0) : $plaintext[0];

				}



				$binary = _bin(md5($context));



				for ($i = 0; $i < 1000; $i++)

				{

					$new = ($i & 1) ? $plaintext : substr($binary, 0, 16);

					if ($i % 3)

					{

						$new .= $salt;

					}

					if ($i % 7)

					{

						$new .= $plaintext;

					}

					$new .= ($i & 1) ? substr($binary, 0, 16) : $plaintext;

					$binary = _bin(md5($new));

				}



				$p = array();

				for ($i = 0; $i < 5; $i++)

				{

					$k = $i + 6;

					$j = $i + 12;

					if ($j == 16)

					{

						$j = 5;

					}

					$p[] = _toAPRMD5((ord($binary[$i]) << 16) | (ord($binary[$k]) << 8) | (ord($binary[$j])), 5);

				}



				return '$apr1$' . $salt . '$' . implode('', $p) . _toAPRMD5(ord($binary[11]), 3);



			case 'md5-hex':

			default:

				$encrypted = ($salt) ? md5($plaintext . $salt) : md5($plaintext);

				return ($show_encrypt) ? '{MD5}' . $encrypted : $encrypted;

		}

	}

	

function getSalt($encryption = 'md5-hex', $seed = '', $plaintext = '')

	{

		// Encrypt the password.

		switch ($encryption)

		{

			case 'crypt':

			case 'crypt-des':

				if ($seed)

				{

					return substr(preg_replace('|^{crypt}|i', '', $seed), 0, 2);

				}

				else

				{

					return substr(md5(mt_rand()), 0, 2);

				}

				break;



			case 'crypt-md5':

				if ($seed)

				{

					return substr(preg_replace('|^{crypt}|i', '', $seed), 0, 12);

				}

				else

				{

					return '$1$' . substr(md5(mt_rand()), 0, 8) . '$';

				}

				break;



			case 'crypt-blowfish':

				if ($seed)

				{

					return substr(preg_replace('|^{crypt}|i', '', $seed), 0, 16);

				}

				else

				{

					return '$2$' . substr(md5(mt_rand()), 0, 12) . '$';

				}

				break;



			case 'ssha':

				if ($seed)

				{

					return substr(preg_replace('|^{SSHA}|', '', $seed), -20);

				}

				else

				{

					return mhash_keygen_s2k(MHASH_SHA1, $plaintext, substr(pack('h*', md5(mt_rand())), 0, 8), 4);

				}

				break;



			case 'smd5':

				if ($seed)

				{

					return substr(preg_replace('|^{SMD5}|', '', $seed), -16);

				}

				else

				{

					return mhash_keygen_s2k(MHASH_MD5, $plaintext, substr(pack('h*', md5(mt_rand())), 0, 8), 4);

				}

				break;



			case 'aprmd5': /* 64 characters that are valid for APRMD5 passwords. */

				$APRMD5 = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';



				if ($seed)

				{

					return substr(preg_replace('/^\$apr1\$(.{8}).*/', '\\1', $seed), 0, 8);

				}

				else

				{

					$salt = '';

					for ($i = 0; $i < 8; $i++)

					{

						$salt .= $APRMD5{rand(0, 63)};

					}

					return $salt;

				}

				break;



			default:

				$salt = '';

				if ($seed)

				{

					$salt = $seed;

				}

				return $salt;

				break;

		}

	}



function genRandomBytes($length = 16)

	{

		$sslStr = '';



		if (

			function_exists('openssl_random_pseudo_bytes')

			&& (version_compare(PHP_VERSION, '5.3.4') >= 0

				|| substr(PHP_OS, 0, 3) !== 'WIN'

			)

		)

		{

			$sslStr = openssl_random_pseudo_bytes($length, $strong);

			if ($strong)

			{

				return $sslStr;

			}

		}



		$bitsPerRound = 2;

		$maxTimeMicro = 400;

		$shaHashLength = 20;

		$randomStr = '';

		$total = $length;



		$urandom = false;

		$handle = null;

		if (function_exists('stream_set_read_buffer') && @is_readable('/dev/urandom'))

		{

			$handle = @fopen('/dev/urandom', 'rb');

			if ($handle)

			{

				$urandom = true;

			}

		}



		while ($length > strlen($randomStr))

		{

			$bytes = ($total > $shaHashLength)? $shaHashLength : $total;

			$total -= $bytes;



			$entropy = rand() . uniqid(mt_rand(), true) . $sslStr;

			$entropy .= implode('', @fstat(fopen( __FILE__, 'r')));

			$entropy .= memory_get_usage();

			$sslStr = '';

			if ($urandom)

			{

				stream_set_read_buffer($handle, 0);

				$entropy .= @fread($handle, $bytes);

			}

			else

			{



				$samples = 3;

				$duration = 0;

				for ($pass = 0; $pass < $samples; ++$pass)

				{

					$microStart = microtime(true) * 1000000;

					$hash = sha1(mt_rand(), true);

					for ($count = 0; $count < 50; ++$count)

					{

						$hash = sha1($hash, true);

					}

					$microEnd = microtime(true) * 1000000;

					$entropy .= $microStart . $microEnd;

					if ($microStart > $microEnd) {

						$microEnd += 1000000;

					}

					$duration += $microEnd - $microStart;

				}

				$duration = $duration / $samples;



				$rounds = (int)(($maxTimeMicro / $duration) * 50);



				$iter = $bytes * (int) ceil(8 / $bitsPerRound);

				for ($pass = 0; $pass < $iter; ++$pass)

				{

					$microStart = microtime(true);

					$hash = sha1(mt_rand(), true);

					for ($count = 0; $count < $rounds; ++$count)

					{

						$hash = sha1($hash, true);

					}

					$entropy .= $microStart . microtime(true);

				}

			}



			$randomStr .= sha1($entropy, true);

		}



		if ($urandom)

		{

			@fclose($handle);

		}



		return substr($randomStr, 0, $length);

	}

	

function genRandomPassword($length = 8)

	{

		$salt = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

		$base = strlen($salt);

		$makepass = '';



		$random = genRandomBytes($length + 1);

		$shift = ord($random[0]);

		for ($i = 1; $i <= $length; ++$i)

		{

			$makepass .= $salt[($shift + ord($random[$i])) % $base];

			$shift += ord($random[$i]);

		}



		return $makepass;

	}



	function _toAPRMD5($value, $count)

	{

		/* 64 characters that are valid for APRMD5 passwords. */

		$APRMD5 = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';



		$aprmd5 = '';

		$count = abs($count);

		while (--$count)

		{

			$aprmd5 .= $APRMD5[$value & 0x3f];

			$value >>= 6;

		}

		return $aprmd5;

	}



function _bin($hex)

	{

		$bin = '';

		$length = strlen($hex);

		for ($i = 0; $i < $length; $i += 2)

		{

			$tmp = sscanf(substr($hex, $i, 2), '%x');

			$bin .= chr(array_shift($tmp));

		}

		return $bin;

	}		





if(!function_exists('register')) {

	function register($message, $class) {

	$spammer2 = 'SK'.rand(199000, 999999);



	//load defaults 

	$siteName = getSetting('siteName');

	$siteLogo = getSetting('siteLogo');

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;

	if(empty($siteLogo)){ $siteLogo = 'media/images/logo.png'; } else {

		$siteLogo = 'media/uploads/'.$siteLogo;

	}	

?>

       

   <div id="login-center">

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> تسجيل الدخول</div>

			<div id="login-form">

            <form method="post" action="" name="login">

                <p><input type="text" name="access_login" placeholder=" أسم المستخدم الخاص بك"></p>

                <p><input type="password" name="access_password" placeholder="كلمه المرور الخاص بك"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >تسجيل الدخول الآن</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>index.php?register&p"><button>أنشاء حساب جديد الآن</button></a></p>

            <p><a href="<?php echo BASE; ?>index.php?reset&p" class="link">استرجاع كلمه المرور</a></p>

			<div id="login-loading"><img src="<?php echo BASE; ?>media/images/spin.gif" /> Processing...</div>

        </div>

		<div id="divide"></div>

		<div id="login-logo">

			<img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" title="<?php echo $siteName; ?>" />

		</div>

    </div> 

<?php

		$name = '';

		$username = '';

		$email = '';

		$phone = '';

		$address = '';

		$currency = '';

		$country = '';

		$isReseller = '';

		$password = '';	



	if(isset($_POST['register'])) {

		$name = $_POST['name'];

		$username = $_POST['username'];

		$email = $_POST['email'];

		$phone = $_POST['phone'];

		$phone = str_replace('+', '', $phone);

		$address = $_POST['address'];

		$city = $_POST['city'];

		$state = $_POST['state'];

		$currency = getSetting('defaultCurrency');

		$country = $_POST['country'];

		$phoneVerified = '1';

		$emailVerified = '0';

		$verificationCode = rand(199999, 999999);

		$password = $_POST['password'];

		$spammer1 = $_POST['spammer1'];

		$spammer = $_POST['spammer'];

		$password1 = $_POST['password2'];

		$salt = genRandomPassword(32);

		$crypt = getCryptedPassword($password, $salt);

		$password2 = $crypt.':'.$salt;		

		

		if(strlen($phone) < 9) {

			$error = 1;

			$message = 'Sorry but your phone number is not supported. Please use a valid Moble Number including your country code .';

			$class = 'red';	

		}

		if($phone == '123456789' || $phone == '12345678' || $phone == '1234567' || $phone == '123456') {

			$error = 1;

			$message = 'Sorry but your phone number is not recorgnized. Please use a valid Moble Number including your country code .';

			$class = 'red';	

		}



		if(emailExist($email)) {

			$error = 1;

			$message = 'Sorry but another user already exist with same email address.';

			$class = 'red';	

		}				

		if($password != $password1) {

			$error = 1;

			$message = 'Sorry but your two password fields does not match. Please checkكلمه المرور الخاص بكs and try again.';

			$class = 'red';	

		}					

		if(usernameExist($username)) {

			$error = 1;

			$message = 'Sorry but another user already exist with same username.';

			$class = 'red';	

		}	

		

		if($spammer != $spammer1) {

			$error = 1;

			$message = 'خطأ في البيانات المدخله راجع رمز التحقق فضلآ.';

			$class = 'red';

		}			

	if(!isset($error)) {	

	//add new account

	$testUnits = getSetting('testUnits');

	$code = rand(199999, 999999);

		$add = mysql_query("INSERT INTO customers (`id`, `name`, `email`, `phone`, `username`, `password`, `address`, `city`, `state`, `country`, `smsPurchase`, `smsBalance`, `isReseller`, `phoneVerified`, `emailVerified`, `verificationCode`, `currency`, `suspended`) 

			VALUES (NULL, '$name', '$email', '$phone', '$username', '$password2', '$address', '$city', '$state', '$country', '$testUnits', '$testUnits', '$isReseller', '1', '$emailVerified', '$code', '$currency', '0');") or die(mysql_error());	

//verify

//send email

		$mail = '<html><body>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Hi '.$name.',</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Thank you for registering at '.getSetting('siteName').'.</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Your account activation link is <b><a href="'.home_base_url().'index.php?verify&token='.$code.'">'.home_base_url().'index.php?verify&token='.$code.'</a></b>.</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">You must follow the above link to complete your registration. </p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '</body></html>';

	$emailSender = getSetting('emailSender');

	$emailFrom = getSetting('companyEmail');		

sendEmail($emailFrom,$emailSender,'Your New SMS Account Activation Link',$email,$mail);

	//get inserted id

	$sql = "SELECT * FROM customers ORDER BY id DEsC LIMIT 1";

	$result = mysql_query($sql);

	$num = mysql_numrows($result);

	$row = mysql_fetch_assoc($result); 

	$userID = $row['id'];					

	//send new password by sms and email

	$newAccountSMS = getSetting('newAccountSMS');

	$newAccountEmail = getSetting('newAccountEmail');

	$newAccountEmailSubject = getSetting('newAccountEmailSubject');

	$smsSender = getSetting('smsSender');

	$emailSender = getSetting('emailSender');

	$emailFrom = getSetting('companyEmail');

	$customer = $userID;



		if(!empty($newAccountSMS)) {

			$mail = str_replace('[USERNAME]', $username, $newAccountSMS);	

			$mail = str_replace('[PASSWORD]', $password, $newAccountSMS);

			$mail = str_replace('[CUSTOMER NAME]', $name, $newAccountSMS);	

			$mail = strtr ($newAccountSMS, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));			

			sendAdminMessage($smsSender,$phone,$mail,'0','Admin','');		

		}



		if(!empty($newAccountEmail)) {

			$mail = str_replace('[USERNAME]', $username, $newAccountEmail);	

			$mail = str_replace('[PASSWORD]', $password, $newAccountEmail);

			$mail = str_replace('[CUSTOMER NAME]', $name, $newAccountEmail);

			$mail = strtr ($newAccountEmail, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));										

			sendEmail($emailFrom,$emailSender,$newAccountEmailSubject,$email,$mail);

		}	

	header('location: '.BASE.'index.php?registered&p');	

	}

	}

?>



<div id="add-new" style="top: 15%; height: 450px;">

	<div id="add-new-head">Register

    <a href="javascript:{}" title="Close" id="closeBox" onClick="document.getElementById('add-new').style.display='none';"><div class="close">X</div></a></div>

     <div class="inside">    

		<div id="mess" style="position: relative; top: 0;">     

            <?php if(!empty($message)) { showMessage($message,$class); } ?>

        </div>        

    <form method="post" action="" enctype="multipart/form-data">

    <table width="100%" border="0" cellspacing="5" cellpadding="0">

      <tr>

        <td align="left" valign="middle">الآسم بالكامل :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="test" name="name" id="name" value="<?php echo $name; ?>" maxlength="200" required="required" placeholder="">                      

        </td>

      </tr>

      <tr>

        <td align="left" valign="middle">أسم المستخدم :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text"  name="username" id="username" value="<?php echo $username; ?>" required="required" placeholder="">                      

        </td>

      </tr>      

      <tr>

        <td align="left" valign="middle">البريد الآلكتروني :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="email" id="email" value="<?php echo $email; ?>" maxlength="200" required="required" placeholder="">                      

        </td>

      </tr>    

      <tr>

        <td align="left" valign="middle">رقم هاتفك مع رمز الدوله بدون +</td>

        <td width="70%" align="left" valign="middle">

        	<input type="number" name="phone" id="phone" value="<?php echo $phone; ?>" maxlength="16" required="required" placeholder="Include country code E.g. 0035712345678">                      

        </td>

      </tr>        

      <tr>

        <td align="left" valign="middle">العنوان بالآنجليزيه :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="address" id="address" value="<?php echo $address; ?>" maxlength="300"  placeholder="">        </td>

      </tr>

      <tr>

        <td align="left" valign="middle">المحافظه :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="city" id="city" value="<?php echo $city; ?>" maxlength="300"  placeholder="">        </td>

      </tr>

      <tr>

        <td align="left" valign="middle">المنطقه :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="state" id="state" value="<?php echo $state; ?>" maxlength="300"  placeholder="">        </td>

      </tr>      

      <tr>

        <td align="left" valign="middle">المنطقه :</td>

        <td width="70%" align="left" valign="middle">

        	<select name="country" id="e1" style="width: 98%;" >

            	<?php getCountryList($country); ?>

            </select>                      

        </td>

      </tr>             

      <tr>

        <td align="left" valign="middle">كلمه المرور :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="password" name="password" required="required" id="password" maxlength="200"  placeholder="A minimun of 8 characters is recommended">                      

        </td>

      </tr>         

      <tr>

        <td align="left" valign="middle">أعاده ادخال كلمه المرور :</td>

        <td width="70%" align="left" valign="middle">

        	<input type="password" name="password2" required="required" id="password2" maxlength="16" placeholder="">                      

        </td>

      </tr>

      <tr>

        <td align="left" valign="middle"></td>

        <td width="70%" align="left" valign="middle">

        	<br />اكتب الآرقام التي تظهر اليك هذه الخدمه مهمه لكي نتأكد من انك شخص حقيقي     <br />

            <div style="margin-left: 10%;width: 100px;height: 40px;line-height: 40px;padding: 2px 10px; background-color:#9CC;font-size: 20px;text-align: center;"><?php echo $spammer2; ?></div>                

        </td>

      </tr>

            <tr>

        <td align="left" valign="middle">اكتب النص هنا</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="spammer" required="required" id="spammer" placeholder="Type exactly what you see here" style="">

            <input type="hidden" name="spammer1" value="<?php echo $spammer2; ?>" />                      

        </td>

      </tr>       

      <tr>

        <td align="left" valign="top">&nbsp;</td>

        <td width="69%" align="left" valign="top">

        <input type="hidden" name="register"  />

        <button class="submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" name="save" value="1" type="submit">أنشأ الحساب الآن</button>

	</form>     

     	<div id="login-loading"><img src="<?php echo BASE; ?>media/images/spin.gif" /> Processing...</div>     

        </td>

      </tr>    

   </table>  

   </div>

 </div>  



	

  </body>

</html>	

 <?php 

 	die();

 //end sho login	

	}

}



if(!function_exists('ResendCode')) {

	function ResendCode($message, $class,$id) {

	$siteName = getSetting('siteName');

	$siteLogo = getSetting('siteLogo');

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;

	if(empty($siteLogo)){ $siteLogo = 'media/images/logo.png'; } else {

		$siteLogo = 'media/uploads/'.$siteLogo;

	}

	

//get code

	$sql = "SELECT * FROM customers WHERE id = '$id'";

	$result = mysql_query($sql);

	$num = mysql_num_rows($result);

	$row = mysql_fetch_assoc($result); 

	$name = $row['name'];

	$code = $row['verificationCode'];

	$email = $row['email'];

//send email

		$mail = '<html><body>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Hi '.$name.',</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Thank you for registering at '.$siteName.'.</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Your account activation link is <b><a href="'.home_base_url().'index.php?verify&token='.$code.'">'.home_base_url().'index.php?verify&token='.$code.'</a></b>.</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">You must follow the above link to complete your registration. </p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '</body></html>';

	$emailSender = getSetting('emailSender');

	$emailFrom = getSetting('companyEmail');		

sendEmail($emailFrom,$emailSender,'Your New SMS Account Activation Link',$email,$mail);		

?>

    	<div id="mess" style="width: 100%;">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

		

        <p style="text-align:center;"><a href="<?php echo BASE; ?>index.php"><button class="submit" style="float:none; margin-top: 140px; width: 200px; height: 50px; font-size: 18px;">Proceed to تسجيل الدخول الآن</button></a></p>

	

  </body>

</html>	

 <?php 

 	die();

	}

}





if(!function_exists('RecoverPassword')) {

	function RecoverPassword($message, $class) {



	//load defaults 

	$siteName = getSetting('siteName');

	$siteLogo = getSetting('siteLogo');

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;

	if(empty($siteLogo)){ $siteLogo = 'media/images/logo.png'; } else {

		$siteLogo = 'media/uploads/'.$siteLogo;

	}	

?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> Resetكلمه المرور الخاص بك</div>

			<div id="login-form">

            <form method="post" action="" name="login">

                <p><input type="email" name="userEmail" placeholder=" Your Email Address"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >تغير او استرجاع لكلمه المرور</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>index.php"><button>اعاده تسجيل الدخول الآن</button></a></p>

			<div id="login-loading"><img src="<?php echo BASE; ?>media/images/spin.gif" /> Processing...</div>

        </div>

		<div id="divide"></div>

		<div id="login-logo">

			<img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" title="<?php echo $siteName; ?>" />

		</div>

    </div> 

	

  </body>

</html>	

 <?php 

 	die();

	}

}

if(!function_exists('AdminRecoverPassword')) {

	function AdminRecoverPassword($message, $class) {



	//load defaults 

	$siteName = getSetting('siteName');

	$siteLogo = getSetting('siteLogo');

	$siteName = 'SMSPanel v1.0';

	$siteLogo = 'media/images/logo.png'; 

?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> Reset Your Admin Password</div>

			<div id="login-form">

            <form method="post" action="" name="login">

                <p><input type="email" name="userEmail" placeholder=" Your Email Address"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >تغير او استرجاع لكلمه المرور</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>administrator/index.php"><button>اعاده تسجيل الدخول الآن</button></a></p>

			<div id="login-loading"><img src="<?php echo BASE; ?>media/images/spin.gif" /> Processing...</div>

        </div>

		<div id="divide"></div>

		<div id="login-logo">

			<img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" title="<?php echo $siteName; ?>" />

		</div>

    </div> 

	

  </body>

</html>	

 <?php 

 	die();

	}

}

class PasswordHash {

	var $itoa64;

	var $iteration_count_log2;

	var $portable_hashes;

	var $random_state;



	function PasswordHash($iteration_count_log2, $portable_hashes)

	{

		$this->itoa64 = './0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';



		if ($iteration_count_log2 < 4 || $iteration_count_log2 > 31)

			$iteration_count_log2 = 8;

		$this->iteration_count_log2 = $iteration_count_log2;



		$this->portable_hashes = $portable_hashes;



		$this->random_state = microtime() . getmypid();

	}



	function get_random_bytes($count)

	{

		$output = '';

		if (($fh = @fopen('/dev/urandom', 'rb'))) {

			$output = fread($fh, $count);

			fclose($fh);

		}



		if (strlen($output) < $count) {

			$output = '';

			for ($i = 0; $i < $count; $i += 16) {

				$this->random_state =

				    md5(microtime() . $this->random_state);

				$output .=

				    pack('H*', md5($this->random_state));

			}

			$output = substr($output, 0, $count);

		}



		return $output;

	}



	function encode64($input, $count)

	{

		$output = '';

		$i = 0;

		do {

			$value = ord($input[$i++]);

			$output .= $this->itoa64[$value & 0x3f];

			if ($i < $count)

				$value |= ord($input[$i]) << 8;

			$output .= $this->itoa64[($value >> 6) & 0x3f];

			if ($i++ >= $count)

				break;

			if ($i < $count)

				$value |= ord($input[$i]) << 16;

			$output .= $this->itoa64[($value >> 12) & 0x3f];

			if ($i++ >= $count)

				break;

			$output .= $this->itoa64[($value >> 18) & 0x3f];

		} while ($i < $count);



		return $output;

	}



	function gensalt_private($input)

	{

		$output = '$P$';

		$output .= $this->itoa64[min($this->iteration_count_log2 +

			((PHP_VERSION >= '5') ? 5 : 3), 30)];

		$output .= $this->encode64($input, 6);



		return $output;

	}



	function crypt_private($password, $setting)

	{

		$output = '*0';

		if (substr($setting, 0, 2) == $output)

			$output = '*1';



		if (substr($setting, 0, 3) != '$P$')

			return $output;



		$count_log2 = strpos($this->itoa64, $setting[3]);

		if ($count_log2 < 7 || $count_log2 > 30)

			return $output;



		$count = 1 << $count_log2;



		$salt = substr($setting, 4, 8);

		if (strlen($salt) != 8)

			return $output;



		# We're kind of forced to use MD5 here since it's the only

		# cryptographic primitive available in all versions of PHP

		# currently in use.  To implement our own low-level crypto

		# in PHP would result in much worse performance and

		# consequently in lower iteration counts and hashes that are

		# quicker to crack (by non-PHP code).

		if (PHP_VERSION >= '5') {

			$hash = md5($salt . $password, TRUE);

			do {

				$hash = md5($hash . $password, TRUE);

			} while (--$count);

		} else {

			$hash = pack('H*', md5($salt . $password));

			do {

				$hash = pack('H*', md5($hash . $password));

			} while (--$count);

		}



		$output = substr($setting, 0, 12);

		$output .= $this->encode64($hash, 16);



		return $output;

	}



	function gensalt_extended($input)

	{

		$count_log2 = min($this->iteration_count_log2 + 8, 24);

		# This should be odd to not reveal weak DES keys, and the

		# maximum valid value is (2**24 - 1) which is odd anyway.

		$count = (1 << $count_log2) - 1;



		$output = '_';

		$output .= $this->itoa64[$count & 0x3f];

		$output .= $this->itoa64[($count >> 6) & 0x3f];

		$output .= $this->itoa64[($count >> 12) & 0x3f];

		$output .= $this->itoa64[($count >> 18) & 0x3f];



		$output .= $this->encode64($input, 3);



		return $output;

	}



	function gensalt_blowfish($input)

	{

		# This one needs to use a different order of characters and a

		# different encoding scheme from the one in encode64() above.

		# We care because the last character in our encoded string will

		# only represent 2 bits.  While two known implementations of

		# bcrypt will happily accept and correct a salt string which

		# has the 4 unused bits set to non-zero, we do not want to take

		# chances and we also do not want to waste an additional byte

		# of entropy.

		$itoa64 = './ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';



		$output = '$2a$';

		$output .= chr(ord('0') + $this->iteration_count_log2 / 10);

		$output .= chr(ord('0') + $this->iteration_count_log2 % 10);

		$output .= '$';



		$i = 0;

		do {

			$c1 = ord($input[$i++]);

			$output .= $itoa64[$c1 >> 2];

			$c1 = ($c1 & 0x03) << 4;

			if ($i >= 16) {

				$output .= $itoa64[$c1];

				break;

			}



			$c2 = ord($input[$i++]);

			$c1 |= $c2 >> 4;

			$output .= $itoa64[$c1];

			$c1 = ($c2 & 0x0f) << 2;



			$c2 = ord($input[$i++]);

			$c1 |= $c2 >> 6;

			$output .= $itoa64[$c1];

			$output .= $itoa64[$c2 & 0x3f];

		} while (1);



		return $output;

	}



	function HashPassword($password)

	{

		$random = '';



		if (CRYPT_BLOWFISH == 1 && !$this->portable_hashes) {

			$random = $this->get_random_bytes(16);

			$hash =

			    crypt($password, $this->gensalt_blowfish($random));

			if (strlen($hash) == 60)

				return $hash;

		}



		if (CRYPT_EXT_DES == 1 && !$this->portable_hashes) {

			if (strlen($random) < 3)

				$random = $this->get_random_bytes(3);

			$hash =

			    crypt($password, $this->gensalt_extended($random));

			if (strlen($hash) == 20)

				return $hash;

		}



		if (strlen($random) < 6)

			$random = $this->get_random_bytes(6);

		$hash =

		    $this->crypt_private($password,

		    $this->gensalt_private($random));

		if (strlen($hash) == 34)

			return $hash;



		# Returning '*' on error is safe here, but would _not_ be safe

		# in a crypt(3)-like function used _both_ for generating new

		# hashes and for validating passwords against existing hashes.

		return '*';

	}



	function CheckPassword($password, $stored_hash)

	{

		$hash = $this->crypt_private($password, $stored_hash);

		if ($hash[0] == '*')

			$hash = crypt($password, $stored_hash);



		return $hash == $stored_hash;

	}

}



if ( ! function_exists('read_file'))

{

	function read_file($file)

	{

		if ( ! file_exists($file))

		{

			return FALSE;

		}



		if (function_exists('file_get_contents'))

		{

			return file_get_contents($file);

		}



		if ( ! $fp = @fopen($file, 'r+'))

		{

			return FALSE;

		}



		flock($fp, LOCK_SH);



		$data = '';

		if (filesize($file) > 0)

		{

			$data =& fread($fp, filesize($file));

		}



		flock($fp, LOCK_UN);

		fclose($fp);



		return $data;

	}

}

 

if ( ! function_exists('write_file'))

{

	function write_file($path, $data, $mode = 'w+')

	{

		if ( ! $fp = @fopen($path, $mode))

		{

			return FALSE;

		}



		flock($fp, LOCK_EX);

		fwrite($fp, $data);

		flock($fp, LOCK_UN);

		fclose($fp);



		return TRUE;

	}

}



function updateSystem($version,$source,$destination) {

	$return = true;

	if(!file_exists($source)) {

		$return = 'Unable to complete system update. Required update files are missing!';	

	}

	copy($source, $destination);

	$file_path = dirname(__FILE__).'/config.inc';

	$resultsd = @chmod($file_path, 0777);

	

	include($file_path);		

	$cv = $sconfig['verssion_id'];

	$data3 = read_file($file_path);

	$data3 = str_replace($cv, $version, $data3);

	write_file($file_path, $data3);

return $return;		

}



function correctAll() {

require(BASE . '/drivers/mrbs/settings.inc');	

}

if ( ! function_exists('read_file'))

{

	function read_file($file)

	{

		if ( ! file_exists($file))

		{

			return FALSE;

		}



		if (function_exists('file_get_contents'))

		{

			return file_get_contents($file);

		}



		if ( ! $fp = @fopen($file, 'r+'))

		{

			return FALSE;

		}



		flock($fp, LOCK_SH);



		$data = '';

		if (filesize($file) > 0)

		{

			$data =& fread($fp, filesize($file));

		}



		flock($fp, LOCK_UN);

		fclose($fp);



		return $data;

	}

}

 

if ( ! function_exists('write_file'))

{

	function write_file($path, $data, $mode = 'w+')

	{

		if ( ! $fp = @fopen($path, $mode))

		{

			return FALSE;

		}



		flock($fp, LOCK_EX);

		fwrite($fp, $data);

		flock($fp, LOCK_UN);

		fclose($fp);



		return TRUE;

	}

}



 

if ( ! function_exists('delete_files'))

{

	function delete_files($path, $del_dir = FALSE, $level = 0)

	{

		// Trim the trailing slash

		$path = rtrim($path, DIRECTORY_SEPARATOR);



		if ( ! $current_dir = @opendir($path))

		{

			return FALSE;



		}



		while (FALSE !== ($filename = @readdir($current_dir)))

		{

			if ($filename != "." and $filename != "..")

			{

				if (is_dir($path.DIRECTORY_SEPARATOR.$filename))

				{

					// Ignore empty folders

					if (substr($filename, 0, 1) != '.')

					{

						delete_files($path.DIRECTORY_SEPARATOR.$filename, $del_dir, $level + 1);

					}

				}

				else

				{

					unlink($path.DIRECTORY_SEPARATOR.$filename);

				}

			}

		}

		@closedir($current_dir);



		if ($del_dir == TRUE AND $level > 0)

		{

			return @rmdir($path);

		}



		return TRUE;

	}

}



if ( ! function_exists('get_filenames'))

{

	function get_filenames($source_dir, $include_path = FALSE, $_recursion = FALSE)

	{

		static $_filedata = array();



		if ($fp = @opendir($source_dir))

		{

			// reset the array and make sure $source_dir has a trailing slash on the initial call

			if ($_recursion === FALSE)

			{

				$_filedata = array();

				$source_dir = rtrim(realpath($source_dir), DIRECTORY_SEPARATOR).DIRECTORY_SEPARATOR;

			}



			while (FALSE !== ($file = readdir($fp)))

			{

				if (@is_dir($source_dir.$file) && strncmp($file, '.', 1) !== 0)

				{

					get_filenames($source_dir.$file.DIRECTORY_SEPARATOR, $include_path, TRUE);

				}

				elseif (strncmp($file, '.', 1) !== 0)

				{

					$_filedata[] = ($include_path == TRUE) ? $source_dir.$file : $file;

				}

			}

			return $_filedata;

		}

		else

		{

			return FALSE;

		}

	}

}





if ( ! function_exists('get_dir_file_info'))

{

	function get_dir_file_info($source_dir, $top_level_only = TRUE, $_recursion = FALSE)

	{

		static $_filedata = array();

		$relative_path = $source_dir;



		if ($fp = @opendir($source_dir))

		{

			// reset the array and make sure $source_dir has a trailing slash on the initial call

			if ($_recursion === FALSE)

			{

				$_filedata = array();

				$source_dir = rtrim(realpath($source_dir), DIRECTORY_SEPARATOR).DIRECTORY_SEPARATOR;

			}



			// foreach (scandir($source_dir, 1) as $file) // In addition to being PHP5+, scandir() is simply not as fast

			while (FALSE !== ($file = readdir($fp)))

			{

				if (@is_dir($source_dir.$file) AND strncmp($file, '.', 1) !== 0 AND $top_level_only === FALSE)

				{

					get_dir_file_info($source_dir.$file.DIRECTORY_SEPARATOR, $top_level_only, TRUE);

				}

				elseif (strncmp($file, '.', 1) !== 0)

				{

					$_filedata[$file] = get_file_info($source_dir.$file);

					$_filedata[$file]['relative_path'] = $relative_path;

				}

			}



			return $_filedata;

		}

		else

		{

			return FALSE;

		}

	}

}

function val($user) {

		$val = 'uggc://jjj.largvagrenpgvir.pbz/NCV/yvprafr.cuc';

		$ud1 = str_rot13($val);		

		$key = $user;

		$product = 'FomeSMSPortal';

		

		$url = $ud1;		

	

		$fields = array(

		   'key'=>$key,

		   'product'=>$product

		);	



	//build the urlencoded data

	$postvars='';

	$sep='';

		foreach($fields as $key=>$value) 

		{ 

	   $postvars.= $sep.urlencode($key).'='.urlencode($value); 

	   $sep='&'; 

	}



	//open connection

	$ch = curl_init();

	

	//set the url, number of POST vars, POST data

	curl_setopt($ch, CURLOPT_URL, $url);

	curl_setopt($ch, CURLOPT_POST, count($fields));

	curl_setopt($ch, CURLOPT_POSTFIELDS, $postvars);

	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

	curl_setopt($ch, CURLOPT_TIMEOUT, 30);

	curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);	

	

	//execute post

	$result = curl_exec($ch);

	$status = curl_getinfo($ch);

	curl_close($ch);		



	if(!$result) {

	  $return = 'FAILED';

	  }

	else {

		$b22 = explode(' ',$result);

		$return = $b22[0];		

	  }  

return $return;

}



function checkKey($key) {

	$host = $_SERVER['SERVER_NAME'];

	$ip = $_SERVER['SERVER_ADDR'];

	$string = $host.$ip;

	$string = str_replace('www.', '', $string);



	$PasswordHash = new PasswordHash(32, 'Vanilla');

	if(!$PasswordHash->CheckPassword($string, $key, 'Vanilla', 'KeyEncript')) {

       	$return = false;		

	}

	else {

		$return = true;

	}	



return $return;

}



$heda = '

<!doctype html>

<html>

<head>

<meta charset="utf-8">

<style type="text/css">

body {

	font: 100%/1.4 Verdana, Arial, Helvetica, sans-serif;

	background-color: #ccc;

	margin: 0;

	padding: 0;

	color: #000;

}





a:link {

	color:#414958;

	text-decoration: underline;

}

a:visited {

	color: #4E5869;

	text-decoration: underline;

}

a:hover, a:active, a:focus {

	text-decoration: none;

}



.container {

	width: 100%;

	//max-width: 1260px;

	min-width: 780px;

	background-color: #ccc;

	margin: 0 auto; 

}



.box {

	width: 60%;

	background-color: white;

	border: solid 3px #06C;

	border-radius: 15px;

	padding: 20px;

	margin: 0 auto;	

	margin-top: 100px;

}



.message {

	width: 80%;

	background-color: transparent;

	border: solid 1px transparent;

	border-radius: 7px;

	padding: 10px;

	margin: 0 auto;	

	font-size: 18px;

	text-align: center;

	margin-bottom: 10px;

}



.success {

	background-color: #CFC;

	color: #030;	

	border-color: #093;

}



.error {

	background-color: #FCC;

	color: #900;	

	border-color: #C93;

}



.info {

	background-color: #E8F1F9;

	color: #003;	

	border-color: #69F;

}



.content {

	padding: 10px 0;

}



.submit, .buy {

	width: 200px;

	height: 45px;

	font-size: 20px;

	color: white;

	font-weight: bold;

	border: solid 1px #000;	

	border-radius: 5px;

}



.submit:hover, .buy:hover {

	background-color: black;

}



.submit {

	background-color: #063;	

}



.buy {

	background-color: #066;	

}



#email, #key {

	width: 80%;

	height: 40px;

	border: 1px solid #777;

	border-radius: 5px;

	margin: 0 auto;

	margin-top: 10px;

	display: block;	

	padding-left: 10px;

	font-size: 18px;

	text-transform: uppercase;

}



p.in {

	text-align: center;

	margin-top: 10px;

	margin-bottom: 20px;	

}

</style>

</head>

';



$incorectLicense = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	The Product key you entered is incorrect. Please check your key and try again

            </div>

            

            <form action="" method="POST">

  

            	<input name="key" type="text" id="key" required value="Your 16 Digits Product Key" maxlength="16" onfocus="if(this.value  == \'Your 16 Digits Product Key\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your 16 Digits Product Key\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal"><button class="buy" type="button" name="buy_key">Obtain License</button></a></p>

			 <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal/license">Redeem my License</a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



$cls = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message success">

            	Congratulations! <br>Your Product key has been validated. Click the button below to start using your new software

            </div>

            

             <p class="in">  <a href=""><button class="submit" name="buy_key">Proceed</button></a></p>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



$tls = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message success">

            	Congratulations! <br>Your Trial key has been validated. You will be allowed to use this product for a trial period of <b>3 Days</b>, after which you will be required to purchase a Product Key.<br> Click the button below to start trying your new software

            </div>

            

             <p class="in">  <a href=""><button class="submit" name="buy_key">Proceed</button></a></p>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



$uls = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	The Product key you entered has already been used with another system. Please obtain a new key and try again

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your 16 Digits Product Key" maxlength="16" onfocus="if(this.value  == \'Your 16 Digits Product Key\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your 16 Digits Product Key\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal"><button class="buy" type="button" name="buy_key">Obtain License</button></a></p>

			 <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal/license">Redeem my License</a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



$checkFailed = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	Unable to validate your key. Please check your internet connection and try again.

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your 16 Digits Product Key" maxlength="16" onfocus="if(this.value  == \'Your 16 Digits Product Key\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your 16 Digits Product Key\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal"><button type="button" class="buy" name="buy_key">Obtain License</button></a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



$vle = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	 but you can no-longer make use of this system due to illegal modifications to the software, which is a major violation to your license terms. <br>Kindly contact us for further instructions on how to resolve this.

            </div>

            

            <form action="" method="POST">             

             <p class="in">  <a href="http://smsexe.life-host.info/ee//fomesmsportal"><button type="button" class="submit" name="buy_key">Contact Support</button></a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



if(@clientArea == true) {

$vle = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	Oops!<br>Something doesn\'t seem right. <br>Please contact the site owner at '.getSetting('siteURL').'. The error code is 4001.

            </div>

            

            <form action="" method="POST">             

             <p class="in">  <a href="'.getSetting('siteURL').'"><button type="button" class="submit" name="buy_key">Contact Owner</button></a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';	

}



$ivl = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	Your Product key is not valid on this server. Please enter a new key to continue

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your 16 Digits Product Key" maxlength="16" onfocus="if(this.value  == \'Your 16 Digits Product Key\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your 16 Digits Product Key\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal"><button class="buy" name="buy_key">Obtain License</button></a></p>

			 

			 <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal/license">Redeem my License</a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



$enterKey = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message info">

            	Please Enter Your Product Key <br>(Make sure you are connected to the internet before you proceed)

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your 16 Digits Product Key" maxlength="16" onfocus="if(this.value  == \'Your 16 Digits Product Key\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your 16 Digits Product Key\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal"><button type="button" class="buy" name="buy_key">Obtain License</button></a></p>

			  <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal/license">Redeem my License</a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';



if(@clientArea == true) {

$enterKey = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message error">

            	Oops!<br>Something doesn\'t seem right. <br>Please contact the site owner. The error code is 4001.

            </div>

            

            <form action="" method="POST">             

             <p class="in">  <a href="'.getSetting('siteURL').'"><button type="button" class="submit" name="buy_key">Contact Owner</button></a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';	

}



$expiredKey = '

<body>

<div class="container">

  <div class="content">

	<div class="box">

        	<div class="message info">

            	Your trial period has ended. Please enter your Product Key to continue <br>(Make sure you are connected to the internet before your proceed)

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your 16 Digits Product Key" maxlength="16" onfocus="if(this.value  == \'Your 16 Digits Product Key\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your 16 Digits Product Key\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> <a target="_blank" href="http://smsexe.life-host.info/ee//fomesmsportal"><button type="button" class="buy" name="buy_key">Obtain License</button></a></p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';

$path_to_settings = '/drivers/mrbs';

$base_path = DOCUMENT_ROOT.$path_to_settings.'/';



function tableExist($tablename) {

	$root = '';

	

	if ( ! file_exists($tablename)) {

		return false;

	} else {

		return true;

	}

}



if(isset($_POST['submit_key'])) {

$key = $_POST['key'];

$host = $_SERVER['SERVER_NAME'];

$ip = $_SERVER['SERVER_ADDR'];

$string = $host.$ip;

$PasswordHash = new PasswordHash(32, 'Vanilla');		

$response = val($key);

if($response == 'OK') {

$encKey = $PasswordHash->HashPassword($string);

$file_path = $base_path.'settings.inc';

include_once($file_path);		

$current = $sett['key'];

$tr = $sett['Server_HTTP_State'];

$results = @chmod($base_path.'settings.inc', 0777); 

$data3 = read_file($base_path.'settings.inc');

$data3 = str_replace($current, $encKey, $data3);

$data3 = str_replace($tr, '54236733', $data3);

write_file($base_path.'settings.inc', $data3);		



echo $cls;

die();

} elseif ($response == 'USED') {



echo $uls;

die();	

} elseif ($response == 'INVALID') {



echo $ivl;	

die();

} elseif ($response == 'TRIAL') {

$encKey = $PasswordHash->HashPassword($string);

$file_path = $base_path.'settings.inc';

include($file_path);		

$current = $sett['key'];

$tr = $sett['Server_HTTP_State'];

$results = @chmod($base_path.'settings.inc', 0777);

$data3 = read_file($base_path.'settings.inc');

$data3 = str_replace($current, $encKey, $data3);

$data3 = str_replace($tr, '54236733', $data3);

write_file($base_path.'settings.inc', $data3);	

$mkt = time()+60*60*24*3;			

setcookie("tdw-ref", $encKey, $mkt, '/');	

				



echo $tls;

die();	

} else {



	

die();

}

 

die();	

}

$state=$sconfig['installed'];

if($state=='INSTALLED') { 

if(tableExist($base_path.'settings.inc')) {

$file_path = $base_path.'settings.inc';

include($file_path);

if(isset($sett) OR count($sett) != 0) {

if(isset($sett['key'])) { 

if($sett['key'] != 'Encrypted_Key') {

$key = $sett['key'];

if(!checkKey($key)) {



echo $ivl;

die();

} else {

$trial = $sett['Server_HTTP_State'];

if($trial == '87425') {

if(!isset($_COOKIE['tdw-ref']) || empty($_COOKIE['tdw-ref'])) {



;

die();

} else {

$allowed = 'ok';	

}

} else {

$allowed = 'ok';	

}										

}	

} else {



echo $enterKey;

die();

}	

} else {



echo $vle;

die();						

}	

} else {



echo $vle;

die();	

}	

} else {



echo $vle;

die();	

} 

}else {

header('location: '.BASE.'install');	 

}



function print_header($day, $month, $year, $area)

{

	global $mrbs_company, $search_str, $locale_warning;

// churchinfo MRBS integration begins	

	global $iNavMethod;

	require_once('../Include/Header-function.php');

// churchinfo MRBS integration ends

	# If we dont know the right date then make it up 

	if(!$day)

		$day   = date("d");

	if(!$month)

		$month = date("m");

	if(!$year)

		$year  = date("Y");

	if (empty($search_str))

		$search_str = "";



	if ($unicode_encoding)

	{

		header("Content-Type: text/html; charset=utf-8");

	}

	else

	{

		# We use $vocab directly instead of get_vocab() because we have

		# no requirement to convert the vocab text, we just output

		# the charset

		header("Content-Type: text/html; charset=".$vocab["charset"]);

	}



	header("Pragma: no-cache");                          // HTTP 1.0

	header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");    // Date in the past



// churchinfo MRBS integration begins	

// Top level menu index counter

	$MenuFirst = 1;

// churchinfo MRBS integration ends	

?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"

                      "http://www.w3.org/TR/html4/loose.dtd">

<?php

// churchinfo MRBS integration begins	

	$sPageTitle = "Resource Booking System";

	Header_head_metatag(); 

// churchinfo MRBS integration ends

   include "style.inc";

?>

    <SCRIPT LANGUAGE="JavaScript">



<!-- Begin



/*   Script inspired by "True Date Selector"

     Created by: Lee Hinder, lee.hinder@ntlworld.com 

     

     Tested with Windows IE 6.0

     Tested with Linux Opera 7.21, Mozilla 1.3, Konqueror 3.1.0

     

*/



function daysInFebruary (year){

  // February has 28 days unless the year is divisible by four,

  // and if it is the turn of the century then the century year

  // must also be divisible by 400 when it has 29 days

  return (((year % 4 == 0) && ( (!(year % 100 == 0)) || (year % 400 == 0))) ? 29 : 28 );

}



//function for returning how many days there are in a month including leap years

function DaysInMonth(WhichMonth, WhichYear)

{

  var DaysInMonth = 31;

  if (WhichMonth == "4" || WhichMonth == "6" || WhichMonth == "9" || WhichMonth == "11")

    DaysInMonth = 30;

  if (WhichMonth == "2")

    DaysInMonth = daysInFebruary( WhichYear );

  return DaysInMonth;

}



//function to change the available days in a months

function ChangeOptionDays(formObj, prefix)

{

  var DaysObject = eval("formObj." + prefix + "day");

  var MonthObject = eval("formObj." + prefix + "month");

  var YearObject = eval("formObj." + prefix + "year");



  if (DaysObject.selectedIndex && DaysObject.options)

    { // The DOM2 standard way

    // alert("The DOM2 standard way");

    var DaySelIdx = DaysObject.selectedIndex;

    var Month = parseInt(MonthObject.options[MonthObject.selectedIndex].value);

    var Year = parseInt(YearObject.options[YearObject.selectedIndex].value);

    }

  else if (DaysObject.selectedIndex && DaysObject[DaysObject.selectedIndex])

    { // The legacy MRBS way

    // alert("The legacy MRBS way");

    var DaySelIdx = DaysObject.selectedIndex;

    var Month = parseInt(MonthObject[MonthObject.selectedIndex].value);

    var Year = parseInt(YearObject[YearObject.selectedIndex].value);

    }

  else if (DaysObject.value)

    { // Opera 6 stores the selectedIndex in property 'value'.

    // alert("The Opera 6 way");

    var DaySelIdx = parseInt(DaysObject.value);

    var Month = parseInt(MonthObject.options[MonthObject.value].value);

    var Year = parseInt(YearObject.options[YearObject.value].value);

    }



  // alert("Day="+(DaySelIdx+1)+" Month="+Month+" Year="+Year);



  var DaysForThisSelection = DaysInMonth(Month, Year);

  var CurrentDaysInSelection = DaysObject.length;

  if (CurrentDaysInSelection > DaysForThisSelection)

  {

    for (i=0; i<(CurrentDaysInSelection-DaysForThisSelection); i++)

    {

      DaysObject.options[DaysObject.options.length - 1] = null

    }

  }

  if (DaysForThisSelection > CurrentDaysInSelection)

  {

    for (i=0; i<DaysForThisSelection; i++)

    {

      DaysObject.options[i] = new Option(eval(i + 1));

    }

  }

  if (DaysObject.selectedIndex < 0) DaysObject.selectedIndex = 0;

  if (DaySelIdx >= DaysForThisSelection)

    DaysObject.selectedIndex = DaysForThisSelection-1;

  else

    DaysObject.selectedIndex = DaySelIdx;

}



  //  End -->

    </SCRIPT>

  </HEAD>

  <body onload="javascript:scrollToCoordinates()">

<?php

// churchinfo MRBS integration begins

	Header_Body_scripts();



	if ($iNavMethod != 2)	{

		Header_body_menu();

	}

	else {

		Header_body_nomenu();

	}

// churchinfo MRBS integration ends

	 if ( $GLOBALS["pview"] != 1 ) { ?>



   <?php # show a warning if this is using a low version of php

       if (substr(phpversion(), 0, 1) <= 3)

	       echo get_vocab("not_php3");

//       if (!empty($locale_warning))

//               echo "[Warning: ".$locale_warning."]";

   ?>



    <TABLE WIDTH="100%">

      <TR>

        <TD BGCOLOR="#5B69A6">

          <TABLE WIDTH="100%" BORDER=0>

            <TR>

              <TD CLASS="banner" BGCOLOR="#C0E0FF">

                <FONT SIZE=4>

                  <B><?php echo $mrbs_company ?></B><BR>

                  <A HREF="index.php"><?php echo get_vocab("mrbs") ?></A>

                </FONT>

              </TD>

              <TD CLASS="banner" BGCOLOR="#C0E0FF">

                <FORM ACTION="day.php" METHOD=GET name="Form1">

                  <FONT SIZE=2>

<?php

   genDateSelector("", $day, $month, $year); // Note: The 1st arg must match the last arg in the call to ChangeOptionDays below.

   if (!empty($area))

        echo "

                    <INPUT TYPE=HIDDEN NAME=area VALUE=$area>\n"

 

?>

	            <SCRIPT LANGUAGE="JavaScript">

                    <!--

                    // fix number of days for the $month/$year that you start with

                    ChangeOptionDays(document.Form1, ''); // Note: The 2nd arg must match the first in the call to genDateSelector above.

                    // -->

                    </SCRIPT>

                    <INPUT TYPE=SUBMIT VALUE="<?php echo get_vocab("goto") ?>">

                  </FONT>

                </FORM>

              </TD>

              <TD CLASS="banner" BGCOLOR="#C0E0FF" ALIGN=CENTER>

                <A HREF="help.php?day=<?php echo $day ?>&month=<?php echo $month ?>&year=<?php echo $year ?>"><?php echo get_vocab("help") ?></A>

              </TD>

              <TD CLASS="banner" BGCOLOR="#C0E0FF" ALIGN=CENTER>

                <A HREF="admin.php?day=<?php echo $day ?>&month=<?php echo $month ?>&year=<?php echo $year ?>"><?php echo get_vocab("admin") ?></A>

              </TD>

              <TD CLASS="banner" BGCOLOR="#C0E0FF" ALIGN=CENTER>

                <A HREF="report.php"><?php echo get_vocab("report") ?></A>

              </TD>

              <TD CLASS="banner" BGCOLOR="#C0E0FF" ALIGN=CENTER>

                <FORM METHOD=GET ACTION="search.php">

                  <FONT SIZE=2>

                    <A HREF="search.php?advanced=1"><?php echo get_vocab("search") ?></A>

                  </FONT>

                  <INPUT TYPE=TEXT   NAME="search_str" VALUE="<?php echo $search_str ?>" SIZE=10>

                  <INPUT TYPE=HIDDEN NAME=day        VALUE="<?php echo $day        ?>"        >

                  <INPUT TYPE=HIDDEN NAME=month      VALUE="<?php echo $month      ?>"        >

                  <INPUT TYPE=HIDDEN NAME=year       VALUE="<?php echo $year       ?>"        >

<?php

   if (!empty($area))

        echo "

                  <INPUT TYPE=HIDDEN NAME=area VALUE=$area>\n"

?>

                </FORM>

              </TD>

<?php

    # For session protocols that define their own logon box...

    if (function_exists('PrintLogonBox'))

   	{

   	PrintLogonBox();

   	}

?>

            </TR>

          </TABLE>

        </TD>

      </TR>

    </TABLE>

<?php } ?>

<?php

}



function toTimeString(&$dur, &$units)

{

	if($dur >= 60)

	{

		$dur /= 60;



		if($dur >= 60)

		{

			$dur /= 60;



			if(($dur >= 24) && ($dur % 24 == 0))

			{

				$dur /= 24;



				if(($dur >= 7) && ($dur % 7 == 0))

				{

					$dur /= 7;



					if(($dur >= 52) && ($dur % 52 == 0))

					{

						$dur  /= 52;

						$units = get_vocab("years");

					}

					else



						$units = get_vocab("weeks");

				}

				else

					$units = get_vocab("days");

			}

			else

				$units = get_vocab("hours");

		}

		else

			$units = get_vocab("minutes");

	}

	else

		$units = get_vocab("seconds");

}





function toPeriodString($start_period, &$dur, &$units)

{

	global $enable_periods;

        global $periods;



        $max_periods = count($periods);



	$dur /= 60;



        if( $dur >= $max_periods || $start_period == 0 )

        {

                if( $start_period == 0 && $dur == $max_periods )

                {

                        $units = get_vocab("days");

                        $dur = 1;

                        return;

                }



                $dur /= 60;

                if(($dur >= 24) && is_int($dur))

                {

                	$dur /= 24;

			$units = get_vocab("days");

                        return;

                }

                else

                {

			$dur *= 60;

                        $dur = ($dur % $max_periods) + floor( $dur/(24*60) ) * $max_periods;

                        $units = get_vocab("periods");

                        return;

		}

        }

        else

		$units = get_vocab("periods");

}







function genDateSelector($prefix, $day, $month, $year)

{

	if($day   == 0) $day = date("d");

	if($month == 0) $month = date("m");

	if($year  == 0) $year = date("Y");

	

	echo "

                  <SELECT NAME=\"${prefix}day\">";

	

	for($i = 1; $i <= 31; $i++)

		echo "

                    <OPTION" . ($i == $day ? " SELECTED" : "") . ">$i";



	echo "

                  </SELECT>



                  <SELECT NAME=\"${prefix}month\" onchange=\"ChangeOptionDays(this.form,'$prefix')\">";



	for($i = 1; $i <= 12; $i++)

	{

		$m = utf8_strftime("%b", mktime(0, 0, 0, $i, 1, $year));

		

		print "

                    <OPTION VALUE=\"$i\"" . ($i == $month ? " SELECTED" : "") . ">$m";

	}



	echo "

                  </SELECT>

	          <SELECT NAME=\"${prefix}year\" onchange=\"ChangeOptionDays(this.form,'$prefix')\">";



	$min = min($year, date("Y")) - 5;

	$max = max($year, date("Y")) + 5;



	for($i = $min; $i <= $max; $i++)

		print "

                    <OPTION VALUE=\"$i\"" . ($i == $year ? " SELECTED" : "") . ">$i";



	echo "

                  </SELECT>";

}



# Error handler - this is used to display serious errors such as database

# errors without sending incomplete HTML pages. This is only used for

# errors which "should never happen", not those caused by bad inputs.

# If $need_header!=0 output the top of the page too, else assume the

# caller did that. Alway outputs the bottom of the page and exits.

function fatal_error($need_header, $message)

{

	if ($need_header) print_header(0, 0, 0, 0);

	echo $message;

	include "trailer.inc";

	exit;

}



# Apply backslash-escape quoting unless PHP is configured to do it

# automatically. Use this for GET/POST form parameters, since we

# cannot predict if the PHP configuration file has magic_quotes_gpc on.

function slashes($s)

{

	if (get_magic_quotes_gpc()) return $s;

	else return addslashes($s);

}



# Remove backslash-escape quoting if PHP is configured to do it with

# magic_quotes_gpc. Use this whenever you need the actual value of a GET/POST

# form parameter (which might have special characters) regardless of PHP's

# magic_quotes_gpc setting.

function unslashes($s)

{

	if (get_magic_quotes_gpc()) return stripslashes($s);

	else return $s;

}



# Return a default area; used if no area is already known. This returns the

# lowest area ID in the database (no guaranty there is an area 1).

# This could be changed to implement something like per-user defaults.

function get_default_area()

{

	global $tbl_area;

	$area = sql_query1("SELECT id FROM $tbl_area ORDER BY area_name LIMIT 1");

	return ($area < 0 ? 0 : $area);

}



# Return a default room given a valid area; used if no room is already known.

# This returns the first room in alphbetic order in the database.

# This could be changed to implement something like per-user defaults.

function get_default_room($area)

{

	global $tbl_room;

	$room = sql_query1("SELECT id FROM $tbl_room WHERE area_id=$area ORDER BY room_name LIMIT 1");

	return ($room < 0 ? 0 : $room);

}



# Get the local day name based on language. Note 2000-01-02 is a Sunday.

function day_name($daynumber)

{

	return utf8_strftime("%A", mktime(0,0,0,1,2+$daynumber,2000));

}



function hour_min_format()

{

        global $twentyfourhour_format;

        if ($twentyfourhour_format)

	{

  	        return "%H:%M";

	}

	else

	{

		return "%I:%M%p";

	}

}



function period_date_string($t, $mod_time=0)

{

        global $periods;



	$time = getdate($t);

        $p_num = $time["minutes"] + $mod_time;

        if( $p_num < 0 ) $p_num = 0;

        if( $p_num >= count($periods) - 1 ) $p_num = count($periods ) - 1;

	# I have made the separater a ',' as a '-' leads to an ambiguious

	# display in report.php when showing end times.

        return array($p_num, $periods[$p_num] . utf8_strftime(", %A %d %B %Y",$t));

}



function period_time_string($t, $mod_time=0)

{

        global $periods;



	$time = getdate($t);

        $p_num = $time["minutes"] + $mod_time;

        if( $p_num < 0 ) $p_num = 0;

        if( $p_num >= count($periods) - 1 ) $p_num = count($periods ) - 1;

        return $periods[$p_num];

}



function time_date_string($t)

{

        global $twentyfourhour_format;



        if ($twentyfourhour_format)

	{

  	        return utf8_strftime("%H:%M:%S - %A %d %B %Y",$t);

	}

	else

	{

	        return utf8_strftime("%I:%M:%S%p - %A %d %B %Y",$t);

	}

}



# Output a start table cell tag <td> with color class and fallback color.

# $colclass is an entry type (A-J), "white" for empty, or "red" for highlighted.

# The colors for CSS browsers can be found in the style sheet. The colors

# in the array below are fallback for non-CSS browsers only.

function tdcell($colclass)

{

	# This should be 'static $ecolors = array(...)' but that crashes PHP3.0.12!

	static $ecolors;

	if (!isset($ecolors)) $ecolors = array("A"=>"#FFCCFF", "B"=>"#99CCCC",

		"C"=>"#FF9999", "D"=>"#FFFF99", "E"=>"#C0E0FF", "F"=>"#FFCC99",

		"G"=>"#FF6666", "H"=>"#66FFFF", "I"=>"#DDFFDD", "J"=>"#CCCCCC",

		"red"=>"#FFF0F0", "white"=>"#FFFFFF");

	if (isset($ecolors[$colclass]))

		echo "<td class=\"$colclass\" bgcolor=\"$ecolors[$colclass]\">";

	else

		echo "<td class=\"$colclass\">";

}



# Display the entry-type color key. This has up to 2 rows, up to 5 columns.

function show_colour_key()

{

	global $typel;

	echo "<table border=0><tr>\n";

	$nct = 0;

	for ($ct = "A"; $ct <= "Z"; $ct++)

	{

		if (!empty($typel[$ct]))

		{

			if (++$nct > 5)

			{

				$nct = 0;

				echo "</tr><tr>";

			}

			tdcell($ct);

			echo "$typel[$ct]</td>\n";

		}

	}

	echo "</tr></table>\n";

}



# Round time down to the nearest resolution

function round_t_down($t, $resolution, $am7)

{

        return (int)$t - (int)abs(((int)$t-(int)$am7)

				  % $resolution);

}



# Round time up to the nearest resolution

function round_t_up($t, $resolution, $am7)

{

	if (($t-$am7) % $resolution != 0)

	{

		return $t + $resolution - abs(((int)$t-(int)

					       $am7) % $resolution);

	}

	else

	{

		return $t;

	}

}




# generates some html that can be used to select which area should be

# displayed.

function make_area_select_html( $link, $current, $year, $month, $day )

{

	global $tbl_area;

	$out_html = "

<form name=\"areaChangeForm\" method=get action=\"$link\">

  <select name=\"area\" onChange=\"document.areaChangeForm.submit()\">";



	$sql = "select id, area_name from $tbl_area order by area_name";

   	$res = sql_query($sql);

   	if ($res) for ($i = 0; ($row = sql_row($res, $i)); $i++)

   	{

		$selected = ($row[0] == $current) ? "selected" : "";

		$out_html .= "

    <option $selected value=\"".$row[0]."\">" . htmlspecialchars($row[1]);

   	}

	$out_html .= "

  </select>



  <INPUT TYPE=HIDDEN NAME=day        VALUE=\"$day\">

  <INPUT TYPE=HIDDEN NAME=month      VALUE=\"$month\">

  <INPUT TYPE=HIDDEN NAME=year       VALUE=\"$year\">

  <input type=submit value=\"".get_vocab("change")."\">

</form>\n";



	return $out_html;

} # end make_area_select_html



function make_room_select_html( $link, $area, $current, $year, $month, $day )

{

	global $tbl_room;

	$out_html = "

<form name=\"roomChangeForm\" method=get action=\"$link\">

  <select name=\"room\" onChange=\"document.roomChangeForm.submit()\">";



	$sql = "select id, room_name from $tbl_room where area_id=$area order by room_name";

   	$res = sql_query($sql);

   	if ($res) for ($i = 0; ($row = sql_row($res, $i)); $i++)

   	{

		$selected = ($row[0] == $current) ? "selected" : "";

		$out_html .= "

    <option $selected value=\"".$row[0]."\">" . htmlspecialchars($row[1]);

   	}

	$out_html .= "

  </select>

  <INPUT TYPE=HIDDEN NAME=day        VALUE=\"$day\"        >

  <INPUT TYPE=HIDDEN NAME=month      VALUE=\"$month\"        >

  <INPUT TYPE=HIDDEN NAME=year       VALUE=\"$year\"      >

  <INPUT TYPE=HIDDEN NAME=area       VALUE=\"$area\"         >

  <input type=submit value=\"".get_vocab("change")."\">

</form>\n";



	return $out_html;

} # end make_area_select_html





# This will return the appropriate value for isdst for mktime().

# The order of the arguments was chosen to match those of mktime.

# hour is added so that this function can when necessary only be

# run if the time is between midnight and 3am (all DST changes

# occur in this period.

function is_dst ( $month, $day, $year, $hour="-1" )

{



	if( $hour != -1  && $hour > 3)

		return( -1 );

	

	# entering DST

	if( !date( "I", mktime(12, 0, 0, $month, $day-1, $year)) && 

	    date( "I", mktime(12, 0, 0, $month, $day, $year)))

		return( 0 ); 



	# leaving DST

	elseif( date( "I", mktime(12, 0, 0, $month, $day-1, $year)) && 

	    !date( "I", mktime(12, 0, 0, $month, $day, $year)))

		return( 1 );

	else

		return( -1 );

}



# if crossing dst determine if you need to make a modification

# of 3600 seconds (1 hour) in either direction

function cross_dst ( $start, $end )

{

	

	# entering DST

	if( !date( "I", $start) &&  date( "I", $end))

		$modification = -3600;



	# leaving DST

	elseif(  date( "I", $start) && !date( "I", $end))

		$modification = 3600;

	else

		$modification = 0;



	return $modification;

}



function backup_tables($host,$user,$pass,$name,$type,$tables = '*')

{

	$return = '';

	$link = mysql_connect($host,$user,$pass);

	mysql_select_db($name,$link);

	

	//get all of the tables

	if($tables == '*')

	{

		$tables = array();

		$result = mysql_query('SHOW TABLES');

		while($row = mysql_fetch_row($result))

		{

			$tables[] = $row[0];

		}

	}

	else

	{

		$tables = is_array($tables) ? $tables : explode(',',$tables);

	}

	

	//cycle through

	foreach($tables as $table)

	{

		$result = mysql_query('SELECT * FROM '.$table);

		$num_fields = mysql_num_fields($result);

		

		$return.= 'DROP TABLE '.$table.';';

		$row2 = mysql_fetch_row(mysql_query('SHOW CREATE TABLE '.$table));

		$return.= "\n\n".$row2[1].";\n\n";

		

		for ($i = 0; $i < $num_fields; $i++) 

		{

			while($row = mysql_fetch_row($result))

			{

				$return.= 'INSERT INTO '.$table.' VALUES(';

				for($j=0; $j<$num_fields; $j++) 

				{

					$row[$j] = addslashes($row[$j]);

					$row[$j] = ereg_replace("\n","\\n",$row[$j]);

					if (isset($row[$j])) { $return.= '"'.$row[$j].'"' ; } else { $return.= '""'; }

					if ($j<($num_fields-1)) { $return.= ','; }

				}

				$return.= ");\n";

			}

		}

		$return.="\n\n\n";

	}

	

	//save file

	$database_backup = '../backups/soa_db_backup'.time().'.sql';

	$handle = fopen($database_backup,'w+');

	fwrite($handle,$return);

	fclose($handle);

	

	//insert to record

	$filename = 'fome_db_backup'.time().'.sql';

	$date = date('Y-m-d H:i:s');



	$add = mysql_query("INSERT INTO backups (`id`, `date`, `type`, `file`) 

			VALUES (NULL, '$date', '$type', '$filename');") or die(mysql_error());

	return $filename;

}



function restore_tables($filename,$host,$username,$password,$database){

	mysql_connect($host, $username, $password) or die('Error connecting to MySQL server: ' . mysql_error());

	mysql_select_db($database) or die('Error selecting MySQL database: ' . mysql_error());



	$templine = '';

	// Read in entire file

	$lines = file($filename);

	// Loop through each line

	foreach ($lines as $line){

		if (substr($line, 0, 2) == '--' || $line == '')

			continue;

	

	$templine .= $line;

		if (substr(trim($line), -1, 1) == ';')	{

			mysql_query($templine) or print('Error performing query \'<strong>' . $templine . '\': ' . mysql_error() . '<br /><br />');

			$templine = '';

		}

	}

}



?>