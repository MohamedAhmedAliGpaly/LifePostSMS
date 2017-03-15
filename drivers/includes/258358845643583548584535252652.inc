<?php defined('SMSPanel') or die('Snooping around is not allowed. Please use the front door');

$version_id = $sconfig['verssion_id'];

/*

File name: 		index.inc

Description:	This is main fuction that does allmost every stuff around here

Developer: 		Chuka Okoye (Mr. White)

Date: 			11/01/201

*/

set_time_limit(0);

function filterSQL($string) {
	return strtr($string, array ('\'' => '""'));	
}

function revertSQL($string) {
	return strtr($string, array ('""' => '\''));	
}

function create_dirs($path)
{
  if (!is_dir($path))
  {
    $directory_path = "";
    $directories = explode("/",$path);
    array_pop($directories);
    
    foreach($directories as $directory)
    {
      $directory_path .= $directory."/";
      if (!is_dir($directory_path))
      {
        mkdir($directory_path);
        chmod($directory_path, 0777);
      }
    }
  }
}

function unzip($src_file, $dest_dir=false, $create_zip_name_dir=true, $overwrite=true) 
{
  if ($zip = zip_open($src_file)) 
  {
    if ($zip) 
    {
      $splitter = ($create_zip_name_dir === true) ? "." : "/";
      if ($dest_dir === false) $dest_dir = substr($src_file, 0, strrpos($src_file, $splitter))."/";
      
      // Create the directories to the destination dir if they don't already exist
      create_dirs($dest_dir);

      // For every file in the zip-packet
      while ($zip_entry = zip_read($zip)) 
      {
        // Now we're going to create the directories in the destination directories
        
        // If the file is not in the root dir
        $pos_last_slash = strrpos(zip_entry_name($zip_entry), "/");
        if ($pos_last_slash !== false)
        {
          // Create the directory where the zip-entry should be saved (with a "/" at the end)
          create_dirs($dest_dir.substr(zip_entry_name($zip_entry), 0, $pos_last_slash+1));
        }

        // Open the entry
        if (zip_entry_open($zip,$zip_entry,"r")) 
        {
          
          // The name of the file to save on the disk
          $file_name = $dest_dir.zip_entry_name($zip_entry);
          
          // Check if the files should be overwritten or not
          if ($overwrite === true || $overwrite === false && !is_file($file_name))
          {
            // Get the content of the zip entry
            $fstream = zip_entry_read($zip_entry, zip_entry_filesize($zip_entry));

            @file_put_contents($file_name, $fstream );
            // Set the rights
            chmod($file_name, 0777);
          }
          
          // Close the entry
          zip_entry_close($zip_entry);
        }       
      }
      // Close the zip-file
      zip_close($zip);
    }
  } 
  else
  {
    return false;
  }  
  return true;
}

function removeUnicde($str)
{
    $accent   = 'ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõöøùúûýýþÿŔŕƒ?';
    $noaccent = 'SOZsozYYuaaaaaaaceeeeiiiidnoooooouuuuybsaaaaaaaceeeeiiiidnoooooouuuyybyRra?';
    $string = strtr(utf8_decode($string),utf8_decode($accent),$noaccent);
    return strtr($string, $accent, $noaccent);
}

function systemInformation() {

}

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

	function showLoginPasswordProtect($message, $class,$reseller=0) {


	//load defaults 
	$siteName = getSetting('siteName');
	$logo = $siteLogo = getSetting('siteLogo');
	
	if($reseller > 0) {
		$siteName = getResellerSetting('business_name',$reseller);
		$logo = getResellerSetting('logo_url',$reseller);
		$siteLogo = BASE.''.getResellerSetting('logo_url',$reseller);
	}	
	if(empty($siteName)): $siteName = 'Sendroid v1.0'; endif;
	if(empty($logo)){ $siteLogo = 'media/images/logo.png'; } else {
		$siteLogo = BASE.'media/uploads/'.$siteLogo;
	}	
$appId = getSetting('facebook_api'); //Facebook App ID
$appSecret = getSetting('facebook_secrete'); // Facebook App Secret
$homeurl = home_base_url().'index.php';  //return to home
$fbPermissions = 'email';  //Required facebook permissions

//Call Facebook API
$facebook = new Facebook(array(
  'appId'  => $appId,
  'secret' => $appSecret

));
?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head">&nbsp; <i class="fa fa-sign-in"></i> تسجيل الدخول</div>
			<div id="login-form">
            
			<p><img src="<?php echo $siteLogo; ?>" alt="<?php echo $siteName; ?>" title="<?php echo $siteName; ?>" /></p>

            <form method="post" action="index.php" name="login">

                <p><input type="text" name="access_login" placeholder=" أسم المستخدم الخاص بك"></p>

                <p><input type="password" name="access_password" placeholder="كلمه المرور الخاص بك"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >تسجيل الدخول الآن</button></p>

            </form>


			<div id="login-loading"> جاري التحقق .....</div>

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

	 $siteName = 'Sendroid SMS Portal v1.0'; 

	 $siteLogo = 'media/images/logo.png'; 

?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head">&nbsp; <i class="fa fa-sign-in"></i> Admin login</div>

			<div id="login-form">
			<p><img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" /></p>

            <form method="post" action="" name="login">

                <p><input type="text" name="access_login" placeholder=" Your Username"></p>

                <p><input type="password" name="access_password" placeholder=" Your Password"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >Login</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>administrator/index.php?reset&p" class="link">Forgot Password</a></p>

			<div id="login-loading"> Authenticating...</div>

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

function getAdminKey($id) {

	$query="SELECT * FROM users WHERE id = '$id'"; 

	$result = mysql_query($query) or die(mysql_error());  

	$row = mysql_fetch_assoc($result); 

	$value = $row['password'];	

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
$code = str_replace('+', '', $code);
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

function getMessageLink($table,$id) {
	$result = mysql_query("SELECT * FROM $table WHERE id = '$id' LIMIT 1") or die(mysql_error());	
	$row = mysql_fetch_assoc($result);
	$return = 'Compose.php';
	if($row['is_mms'] > 0) {
		$return = 'MMS.php';
	}
	if($row['is_unicode'] > 0) {
		$return = 'Unicode.php';
	}
return $return;	
}
function messageLength($textMessage,$unicode) {
	$test_Len = strlen($textMessage);
	if($unicode > 0) {
		$test_Len = mb_strlen($textMessage);
	}
	$lines = substr_count( $textMessage, "\n" );
	$messageLength = $test_Len - $lines;
return $messageLength;	
}
function  smsCost($recipientList,$textMessage,$customer=0) {
	//get message lenght 
	$requiredCredit = 0;
	$messageLenght = messageLength($textMessage,0);
	$smsLength = "160";
	if(messageLength($textMessage) > 160){
		$smsLength = 145;
	}

	//get price based on destination country
	$pricelist =  getSetting('smsCost');
	if($customer > 0) {
		$userPricelist  = userData('custom_rate',$customer);
		if(!empty($userPricelist) && $userPricelist != 'not available' && $userPricelist != ' ') {
			$pricelist = $userPricelist;	
		}
	}
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



function  smsUnicodeCost($recipientList,$textMessage,$customer=0) {
	//get message lenght 
	$requiredCredit = 0;
	$messageLenght = messageLength($textMessage,1);
	$smsLength = "70";

	//get price based on destination country
	$pricelist =  getSetting('smsCost');
	if($customer > 0) {
		$userPricelist  = userData('custom_rate',$customer);
		if(!empty($userPricelist)) {
			$priceList = $userPricelist;	
		}
	}
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
	if($requiredCredit > 1) {
		$requiredCredit = $requiredCredit -1;	
	}	
	if($requiredCredit > 1) {
		$requiredCredit = $requiredCredit -1;	
	}	
	return $requiredCredit;
}



function countMessageRecipient($id) {

	$sql = "SELECT sum((LENGTH(recipient)+1) - LENGTH(REPLACE(recipient, ',', ''))) as value1 FROM sentmessages WHERE id = '$id'";

	$result = mysql_query($sql) or die(mysql_error());

	$row = mysql_fetch_assoc($result);

	return $num = round($row['value1']);		

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

function isValidURL($url) {
	if (filter_var($url, FILTER_VALIDATE_URL) === FALSE) {
		return false;
	} else {
		return true;	
	}
}



function getInsertedID($table) {
		$result = mysql_query("SELECT * FROM $table ORDER BY id DESC LIMIT 1"); 
		$row = mysql_fetch_assoc($result);
		$id = $row['id'];	
	return $id;	
}

class smpp {

  var $socket=0;
  var $seq=0;
  var $debug=0;
  var $data_coding=0;
  var $timeout = 30;

  //////////////////////////////////////////////////
  function send_pdu($id,$data) {

    // increment sequence
    $this->seq +=1;
    // PDU = PDU_header + PDU_content
    $pdu = pack('NNNN', strlen($data)+16, $id, 0, $this->seq) . $data;
    // send PDU
    fputs($this->socket, $pdu);

    // Get response length
    $data = fread($this->socket, 4);
    if($data==false) die("\nSend PDU: Connection closed!");
    $tmp = unpack('Nlength', $data);
    $command_length = $tmp['length'];
    if($command_length<12) return;

    // Get response 
    $data = fread($this->socket, $command_length-4);
    $pdu = unpack('Nid/Nstatus/Nseq', $data);
    if($this->debug) print "\n< R PDU (id,status,seq): " .join(" ",$pdu) ;

    return $pdu;
  }

  //////////////////////////////////////////////////
  function open($host,$port,$system_id,$password) {

    // Open the socket
    $this->socket = fsockopen($host, $port, $errno, $errstr, $this->timeout);
    if ($this->socket===false)
       die("$errstr ($errno)<br />");
    if (function_exists('stream_set_timeout'))
       stream_set_timeout($this->socket, $this->timeout); // function exists for php4.3+
    if($this->debug) print "\n> Connected" ;


    // Send Bind operation
    $data  = sprintf("%s\0%s\0", $system_id, $password); // system_id, password 
    $data .= sprintf("%s\0%c", "", 0x34);  // system_type, interface_version
    $data .= sprintf("%c%c%s\0", 5, 0, ""); // addr_ton, addr_npi, address_range 

    $ret = $this->send_pdu(2, $data);
    if($this->debug) print "\n> Bind done!" ;

    return ($ret['status']==0);
  }

  //////////////////////////////////////////////////
  function submit_sm($source_addr,$destintation_addr,$short_message,$optional='') {

    $data  = sprintf("%s\0", ""); // service_type
    $data .= sprintf("%c%c%s\0", 5,0,$source_addr); // source_addr_ton, source_addr_npi, source_addr
    $data .= sprintf("%c%c%s\0", 1,1,$destintation_addr); // dest_addr_ton, dest_addr_npi, destintation_addr
    $data .= sprintf("%c%c%c", 0,0,0); // esm_class, protocol_id, priority_flag
    $data .= sprintf("%s\0%s\0", "",""); // schedule_delivery_time, validity_period
    $data .= sprintf("%c%c", 0,0); // registered_delivery, replace_if_present_flag
    $data .= sprintf("%c%c", $this->data_coding,0); // data_coding, sm_default_msg_id
    $data .= sprintf("%c%s", strlen($short_message), $short_message); // sm_length, short_message
    $data .= $optional;

    $ret = $this->send_pdu(4, $data);
    return ($ret['status']==0);
  }

  //////////////////////////////////////////////////
  function close() {

    $ret = $this->send_pdu(6, "");
    fclose($this->socket);
    return true;
  }

  //////////////////////////////////////////////////
  function send_long($source_addr,$destintation_addr,$short_message,$utf=0,$flash=0) {

    if($utf)
      $this->data_coding=0x08;

    if($flash)
      $this->data_coding=$this->data_coding | 0x10;

    $size = strlen($short_message);
    if($utf) $size+=20;
	
		if ($size<160) { // Only one part :)
		  $this->submit_sm($source_addr,$destintation_addr,$short_message);
	
		} else { // Multipart
		  $sar_msg_ref_num =  rand(1,255);
		  $sar_total_segments = ceil(strlen($short_message)/130);
	
		  for($sar_segment_seqnum=1; $sar_segment_seqnum<=$sar_total_segments; $sar_segment_seqnum++) {
			$part = substr($short_message, 0 ,130);
			$short_message = substr($short_message, 130);
	
			$optional  = pack('nnn', 0x020C, 2, $sar_msg_ref_num);
			$optional .= pack('nnc', 0x020E, 1, $sar_total_segments);
			$optional .= pack('nnc', 0x020F, 1, $sar_segment_seqnum);
	
			if ($this->submit_sm($source_addr,$destintation_addr,$part,$optional)===false)
			   return false;			
		  }
    }
   return true;
  }
}

function gatewayRealName($id) {
	$sql = "SELECT * FROM gateways WHERE id = '$id'";
	$result = mysql_query($sql);
	$row = mysql_fetch_assoc($result);
	return $row['name'];
}
function getNumberRoute($phone,$customer) {
	$phone = strtr ($phone, array ('+' => ''));	
	$sql = "SELECT * FROM gateways WHERE param3 != '' ORDER BY id DESC";
	$result = mysql_query($sql) or die(mysql_error());
	$num = mysql_num_rows($result);	
	
	for($i = 0; $i < $num; $i++){
		$id = mysql_result($result,$i,'id');
		$param3 = mysql_result($result,$i,'param3');	
	    $destinationList = explode(",", $param3);
		for($j=0; $j < count($destinationList); $j++){
			$len = strlen($destinationList[$j]); 
			if(substr($phone,0,$len) == $destinationList[$j] && $len > 0 && !empty($destinationList[$j])){
				$gateway = $id; 
				break;
			} else {
			$gateway  = 0;	
			}

		} 	
		if($gateway > 0) { break; }
	}
	if($gateway < 1) {
		$gateway  = getSetting('activeGateway');	
	}
	//check if assigned 
	$sql = "SELECT * FROM customers WHERE id = '$customer'";
	$result = mysql_query($sql);
	$row = mysql_fetch_assoc($result); 
	$temp_gateway = $row['gateway'];
	if($temp_gateway > 0) {
		$gateway = $temp_gateway;	
	}	
	return gatewayRealName($gateway);	
}

function registerApp($email,$company,$app) {
//check update
$getUrl = 'http://lifehost.info/api/sms.php/API/index.php?register_software&app='.$app.'&email='.$email.'&client='.$company;
$curl = curl_init();
curl_setopt($curl, CURLOPT_URL, $getUrl);
curl_setopt($curl, CURLOPT_HTTPGET, TRUE);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
$response = curl_exec($curl);	
}

function getDestinationRoute($phone,$customer=0,$unicode=0) {
	$phone = strtr ($phone, array ('+' => ''));	
	$sql = "SELECT * FROM gateways WHERE param3 != '' ORDER BY id DESC";
	$result = mysql_query($sql) or die(mysql_error());
	$num = mysql_num_rows($result);	
	
	for($i = 0; $i < $num; $i++){
		$id = mysql_result($result,$i,'id');
		$param3 = mysql_result($result,$i,'param3');	
	    $destinationList = explode(",", $param3);
		for($j=0; $j < count($destinationList); $j++){
			$len = strlen($destinationList[$j]); 
			if(substr($phone,0,$len) == $destinationList[$j] && $len > 0 && !empty($destinationList[$j])){
				$gateway = $id; 
				break;
			} else {
			$gateway  = 0;	
			}

		} 	
		if($gateway > 0) { break; }
	}
	if($gateway < 1) {
		$gateway  = getSetting('activeGateway');	
		if($unicode > 0) {
			$gateway = getSetting('activeUnicodeGateway');	
		}
	}
	//check if assigned 
	$sql = "SELECT * FROM customers WHERE id = '$customer'";
	$result = mysql_query($sql);
	$row = mysql_fetch_assoc($result); 
	$temp_gateway = $row['gateway'];
	if($temp_gateway > 0) {
		$gateway = $temp_gateway;	
	}	
	return $gateway;
}

function getNumberCountry($phone) {
	$phone = strtr ($phone, array ('+' => ''));	
	$sql = "SELECT * FROM countrylist ORDER BY id DESC";
	$result = mysql_query($sql) or die(mysql_error());
	$num = mysql_num_rows($result);	
	
	for($i = 0; $i < $num; $i++){
		$code = mysql_result($result,$i,'code');
		$code = ltrim($code);
		$code = strtr ($code, array ('+' => ''));	
		$title = mysql_result($result,$i,'name');	
		$title = preg_replace("/[*0-9+]/","",$title);
		$name = ltrim($title);
		$len = strlen($code); 
		if(substr($phone,0,$len) == $code && $len > 0 && !empty($code)){
			$country = $title; 
			break;
		} else {
			$country  = 'Unknown';	
		}
	}
	return $country;	
}

function get_string_between($string, $start, $end){
    $r = explode($end, $string);
    if (isset($r[0])){
        $r = end(explode($start, $r[0]));
		if ((stripos($r,'http') !== false)){
			$r = end(explode('?', $r));
		}
        return $r;
    }
    return '';
}

function phoneBlacklisted($name) {
	$name = $pricelist = str_replace('+', '' , $name);
	$sql = "SELECT * FROM blacklist WHERE phone = '$name'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	if($num >0) {
		return true;
	} else {
		return false;	
	}
}


function countCountryMessage($country,$month,$customer=0) {
	$sql = "SELECT * FROM messagedetails WHERE country = '$country' AND date LIKE '%$month%'";
	if($customer > 0) {
	$sql = "SELECT * FROM messagedetails m JOIN sentmessages s ON m.queue_id = s.id WHERE m.country = '$country' AND m.date LIKE '%$month%' AND s.customer = '$customer'";	
	} 
	$result = mysql_query($sql) or die(mysql_error());
	return $num = mysql_num_rows($result);	
}

function countDateMessage($country,$month,$customer=0) { 
	$sql = "SELECT * FROM messagedetails WHERE date = '$month'";
	if($customer > 0) {
	$sql = "SELECT * FROM messagedetails m JOIN sentmessages s ON m.queue_id = s.id WHERE m.date = '$month' AND s.customer = '$customer'";	
	}
	$result = mysql_query($sql) or die(mysql_error());
	return $num = mysql_num_rows($result);	
}


function countCountrySentMessage($country,$month,$customer=0) {
	$sql = "SELECT * FROM messagedetails WHERE country = '$country' AND date LIKE '%$month%'  AND status = 'Sent'";
	if($customer > 0) {
	$sql = "SELECT * FROM messagedetails m JOIN sentmessages s ON m.queue_id = s.id WHERE m.country = '$country' AND m.date LIKE '%$month%' AND s.customer = '$customer' AND m.status = 'Sent'";	
	}
	$result = mysql_query($sql) or die(mysql_error());
	return $num = mysql_num_rows($result);	
}

function countDateSentMessage($country,$month,$customer=0) {
	$sql = "SELECT * FROM messagedetails WHERE date = '$month' AND status = 'Sent'";
	if($customer > 0) {
	$sql = "SELECT * FROM messagedetails m JOIN sentmessages s ON m.queue_id = s.id WHERE m.date = '$month' AND s.customer = '$customer' AND m.status = 'Sent'";	
	}
	$result = mysql_query($sql) or die(mysql_error());
	return $num = mysql_num_rows($result);	
}


function sendMessage($senderID,$recipientList,$textMessage,$customer,$sentFrom,$IP='',$unicode=1,$mms=0,$media='',$saved=0,$mes_id=0) {
	$textMessage = strtr($textMessage, array ('""' => '\''));	
	$error = 0;
	if($unicode < 1) {	
		$textMessage = removeUnicde($textMessage);
	}
	//check for MMS
	if($mms > 0 && getSetting('MMSEnabled') < 1) {
		$error = 1;
		$return = 'Unable to send message! MMS feature is not supported';
	}
	
	//check for unicode
	if($unicode > 0 && getSetting('unicodeSMS') < 0) {
		$error = 1;
		$return = 'Unable to send message! UNICODE feature is not supported';
	}
	
	//check for valid media URL
	if($mms > 0 && !isValidURL($media)) {
//		$error = 1;
//		$return = 'Unable to send message! Multimedia URL Format is not valid. Use a valid URL';
	}
		//get gateway settings 
	$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );

	$senderID2 = $senderID;
	$activeGateway = getDestinationRoute($recipientList,$customer,$unicode);
	if($unicode > 1) {
		//$activeGateway = getSetting('activeUnicodeGateway');
	}	
	if($mms > 0) {
		$activeGateway = getSetting('activeMMSGateway');
	}
	
	//remove plus sign
	if($activeGateway != 2) {
	$recipientList = strtr ($recipientList, array ('+' => ''));	
	}	
	
	$sendURL = setGateway($activeGateway,'unicodeAPI');
	if($unicode > 0) {
		$sendURL = setGateway($activeGateway,'unicodeAPI');	
	}	
	if($mms > 0) {
		$sendURL = setGateway($activeGateway,'mmsAPI');
	}
	$sendURL2 = $sendURL;
	if(empty($sendURL)) {
		$error = 1;
		$return = 'Unable to send message! No gateway API provided for this message type';
	}
	$batchSize = setGateway($activeGateway,'batchSize');
	$successWord = setGateway($activeGateway,'successWord');
	$smppServer = setGateway($activeGateway,'smppServer');
	$smppUsername = setGateway($activeGateway,'smppUsername');
	$smppPassword = setGateway($activeGateway,'smppPassword');
	$smppPort = setGateway($activeGateway, 'port');
		
if($error < 1) {	

	
	if(setGateway($activeGateway,'isSMPP') > 0 && !empty($smppServer) && !empty($smppUsername) && !empty($smppPassword) ) { //use SMPP
		$s = new smpp();
		$s->debug=0;	
		$s->open($smppServer, 2775, $smppUsername, $smppPassword);
		if($unicode == 0) {
			$utf = true;
		} else {
			$utf = true;

		}	
		$s->close();
		$response = $successWord;
	} else {		//use cURL
			$sendURL = setGateway($activeGateway,'unicodeAPI');
		if($unicode > 0) {
			$sendURL = setGateway($activeGateway,'unicodeAPI');	
		}	
		if($mms > 0) {
			$sendURL = setGateway($activeGateway,'mmsAPI');
			$sendURL = str_replace('[MEDIA]', urlencode($media), $sendURL);	
		}
		
		$sendURL = str_replace('[SENDER]', urlencode($senderID), $sendURL);
		$sendURL = str_replace('[TO]', urlencode($recipientList), $sendURL);
		$sendURL = str_replace('[MESSAGE]', urlencode($textMessage), $sendURL);
		if($unicode > 1) { //cpnver to unicode
			$textMessage2 = utf8_to_unicode($textMessage);
			$textMessage2 = utf8_to_unicode($textMessage);	
			$sendURL = str_replace('[MESSAGE]', urlencode($textMessage), $sendURL);
		}
		$url = $sendURL;
		$gatewayAuth = setGateway($activeGateway,'auth_key');
		if($activeGateway == 2) { //twilio
			error_reporting(0);
			$auths = explode(':',$gatewayAuth);
			$AccountSid = $auths[0]; 
			$AuthToken = $auths[1]; 
			$version = "2010-04-01"; // Twilio REST API version
			$client = new Services_Twilio($AccountSid, $AuthToken); //initialise the Twilio client
			try{
				if($mms > 0) {
					$message = $client->account->messages->create(array( 

					));
				} else {
					$message = $client->account->messages->create(array( 

					));	
				}
			$response = 'Sent Successfully';	
			}
			catch (Services_Twilio_RestException $e) {
				$response =  $e->getMessage();
			} 
		}  elseif ($activeGateway == 1) {
			$textMessage2 = urlencode($textMessage);
			$data = array("from" => "$senderID", "to" => "$recipientList", "text" => "$recipientList");
			$data_string = json_encode($data);
			$auth = setGateway($activeGateway,'auth_key');
			$authenticate = base64_encode($auth);
				
			$ch = curl_init('');
			curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
			curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
			curl_setopt($ch, CURLOPT_HTTPHEADER, array(
				'Content-Type: application/json',
				'authorization: Basic "'.$authenticate.'"',
				'Accept: application/json',
				'Content-Length: ' . strlen($data_string))
			);
			curl_setopt($ch, CURLOPT_TIMEOUT, 15);
			curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 15);
			$response = curl_exec($ch);
			curl_close($ch);			
		} else { //others
			$method = setGateway($activeGateway,'method');
			if($method == 'tot') {
				$url = $sendURL2;
				$senderVal = get_string_between($url, '&', '=[SENDER]');
				$toVal = get_string_between($url, '&', '=[TO]');
				$messageVal = get_string_between($url, '&', '=[TO]');

				$data_string = json_encode($data);
				if(!empty($gatewayAuth)) { //gateway needs authentication
					$authenticate = setGateway($activeGateway,'auth_key');
					$encode = setGateway($activeGateway,'64_encode');
					if($encode > 0) { //auth needs encoduing
						$authenticate = base64_encode($gatewayAuth);
					}
				}
				$url = explode('?',$url);
				$url = $url[0]; 
				$ch = curl_init($url);
				curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
				curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
				curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
				if(!empty($gatewayAuth)) { 
					curl_setopt($ch, CURLOPT_HTTPHEADER, array(
						'Content-Type: application/json',
						'authorization: Basic "'.$authenticate.'"',
						'Accept: application/json',
						'Content-Length: ' . strlen($data_string))
					);
				} else {
					curl_setopt($ch, CURLOPT_HTTPHEADER, array(
						'Content-Type: application/json',
						'Accept: application/json',
						'Content-Length: ' . strlen($data_string))
					);					
				}
				curl_setopt($ch, CURLOPT_TIMEOUT, 15);
				curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 15);
				$response = curl_exec($ch);
				if(curl_error($ch)) {
					$response = curl_error($ch);	
				}				
				curl_close($ch);					
			} else {  //use normal GET
				$ch = curl_init($url);	
				if(!empty($gatewayAuth)) { //gateway needs authentication
					$auth = setGateway($activeGateway,'auth_key');
					$auths = explode(':',$gatewayAuth);
					$username = $auths[0]; 
					$password = $auths[1];
					curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_BASIC); 
					curl_setopt($ch, CURLOPT_USERPWD, $username . ":" . $password);
				}
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);	
				curl_setopt($ch, CURLOPT_FAILONERROR, true);	
				curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
				$response = curl_exec($ch);
				if(curl_error($ch)) {
					$response = curl_error($ch);	
				}
				curl_close($ch);	
			} //end of get
		}
	}
		$status = 'Failed';	
		if(!$response) {
			$return = 'Connection to Gateway Failed. Please try again later';  //. curl_error($ch)
		  } else {
			 $return = 'Message Sending Failed. Unknown Gateway Error';
				if((stripos($response,$successWord) !== false)) {
				   $return = 'Message Sent Successfully';
				   $status = 'Sent';
				   if(function_exists('get_message_id')) {
					   get_message_id($response);
				   }
			   } else {
				  $return = 'ffffffffffffffffffffffffffffack: '.$response;
			   }
		  }  
}

$error = mysql_real_escape_string($return);
mysql_query("UPDATE `messagedetails` SET `status` = '$status' WHERE `id` = '$mes_id'") or die(mysql_error());
mysql_query("UPDATE `messagedetails` SET `error` = '$error' WHERE `id` = '$mes_id'") or die(mysql_error());

//save messsage if necessary
	if($saved < 1){	  
		$messageLenght = messageLength($textMessage,$unicode);
		$messageLenght = mceil($messageLenght/160);
		$unitsUsed = smsCost($recipientList,$textMessage,$customer);
		if($unicode > 1) {
			$messageLenght = mceil($messageLenght/70);
			$unitsUsed = smsUnicodeCost($recipientList,$textMessage,$customer);			
		}
		if($status == 'Failed') {
			$unitsUsed = 0;
		}
		//add to sent items as failed
		$now = date("Y-m-d H:i:s");
	$reseller = userData('reseller',$customer);	
	if(empty($reseller)) {
		$reseller = 0;	
	}
	$nedOn = date('Y-m-d');
	$gateway = getNumberRoute($recipientList,$customer);
	$country = getNumberCountry($recipientList);	
		$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`,`reseller`) 
		VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$messageLenght', '$status', '$unitsUsed', '$sentFrom', '$IP', '$error','$reseller');");	
		$queue_id  =  getInsertedID('sentmessages');
		
		$add = mysql_query("INSERT INTO messagedetails (`id`, `message`, `sender`, `recipient`, `status`, `queue_id`,`is_mms`,`is_unicode`,`error`, `date`, `country`, `gateway`) 
				VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$status', '$queue_id','$mms','$unicode','$error', '$nedOn', '$country', '$gateway');");	
	} 
	return $return; 
}



function sendQueuedMessage($id) {
	//fetch message
	$sql = "SELECT * FROM sentmessages WHERE id = '$id'";
	$result = mysql_query($sql) or die(mysql_error());
	$row = mysql_fetch_assoc($result);
	
	$error = 0;
	$now = date("Y-m-d H:i:s");
	mysql_query("UPDATE `sentmessages` SET 
	`status` = 'Processing'
	 WHERE `id` = '$id'");
		 
	$customer = $userID = $row['customer'];
	$sentFrom = $row['sentFrom'];
	$Unicode = $row['is_unicode'];
	$recipientList = removeDuplicate($row['recipient']);

	 
	//send massage in batches
	$batchSize = 1;
	$currentList = $recipientcount = explode(',',$recipientList);
	$textMessage = $row['message'];	
	$senderID = $row['senderID'];
	$toNumber = $recipientList;
	$unicode = $row['is_unicode'];
	$mms = $row['is_mms'];
	$media = $row['media'];
	
	$partial_sent = 0;
	$sendingResponse2 = 'Some of your Messages Were Not Sent';
	if($error < 1) {	
		if(count($recipientcount) > $batchSize){			
			foreach($currentList as $to) {
				//insert into messageDetails table
				$status = 'Processing';
				$queue_id = $id;
			$gateway = getNumberRoute($to,$customer);
			$country = getNumberCountry($to);
			$nedOn = date('Y-m-d');
			$add = mysql_query("INSERT INTO messagedetails (`id`, `message`, `sender`, `recipient`, `status`, `queue_id`,`is_mms`,`is_unicode`, `gateway`, `country`, `date`) 
			VALUES (NULL, '$textMessage', '$senderID', '$to', '$status', '$queue_id','$mms','$unicode','$gateway','$country', '$nedOn');");
				$mesd_id = mysql_insert_id();
				$mesd_id  =  getInsertedID('messagedetails');
				//check balance
				$hasBalance = 1;
				if($customer > 0) {
					$smsBalance = userData('smsBalance',$customer);	
					$requiredCredit = smsCost($to,$textMessage,$customer);
					if($unicode > 0) {
						$requiredCredit = smsUnicodeCost($to,$textMessage,$customer);
					}	
					if(($smsBalance < 0) || ($smsBalance < $requiredCredit)) {						
						$hasBalance = 0;
					}
				} 
				if($hasBalance < 1) {
					$sendingResponse = 'Insufficient Balance';
				}
				else				
				if(phoneBlacklisted($to)) {
					$sendingResponse = 'Recipient Blacklisted';
				} else {
				$sendingResponse = sendMessage($senderID,$to,$textMessage,$customer,$sentFrom,$IP,$unicode,$mms,$media,1,$mesd_id);
				}
				if(strpos($sendingResponse,'Message Sent Successfully') !== false) {
					$unitsUsed = smsCost($to,$textMessage,$customer);
					if($unicode > 0) {
						$unitsUsed = smsUnicodeCost($to,$textMessage,$customer);
					}
					
					if($customer > 0) {
						$smsBalance = userData('smsBalance',$customer);
						$newBalance = $smsBalance - $unitsUsed;
						mysql_query("UPDATE `customers` SET `smsBalance` = '$newBalance' WHERE `id` = '$customer'");
					}
					mysql_query("UPDATE `sentmessages` SET `units` = units + '$unitsUsed' WHERE `id` = '$id'");
					mysql_query("UPDATE `messagedetails` SET `status` = 'Sent' WHERE `id` = '$mesd_id'");
				} else {
					$partial_sent = 1;	
					$sendingResponse2 = $sendingResponse;
					$escaped_error = mysql_real_escape_string($sendingResponse);
					mysql_query("UPDATE `messagedetails` SET `status` = 'Failed' WHERE `id` = '$mesd_id'");
					mysql_query("UPDATE `messagedetails` SET `error` = '$escaped_error' WHERE `id` = '$mesd_id'");
				}
			} 
		} else { 
			//insert into messageDetails table
			$status = 'Processing';
			$queue_id = $id;
			$to = $toNumber;
			$gateway = getNumberRoute($to,$customer);
			$country = getNumberCountry($to);
			$nedOn = date('Y-m-d');
			$add = mysql_query("INSERT INTO messagedetails (`id`, `message`, `sender`, `recipient`, `status`, `queue_id`,`is_mms`,`is_unicode`, `gateway`, `country`, `date`) 
			VALUES (NULL, '$textMessage', '$senderID', '$to', '$status', '$queue_id','$mms','$unicode','$gateway','$country', '$nedOn');");
			$mesd_id = mysql_insert_id();
			$mesd_id  =  getInsertedID('messagedetails');
			//check balance
			$hasBalance = 1;
			if($customer > 0) {
				$smsBalance = userData('smsBalance',$customer);	
				$requiredCredit = smsCost($to,$textMessage,$customer);
				if($unicode > 0) {
					$requiredCredit = smsUnicodeCost($to,$textMessage,$customer);
				}	
				if(($smsBalance < 0) || ($smsBalance < $requiredCredit)) {						
					$hasBalance = 0;
				}
			} 
			if($hasBalance < 1) {
				$sendingResponse = 'Insufficient Balance';
			}
			else			
			if(phoneBlacklisted($to)) {
				$sendingResponse = 'Recipient Blacklisted';
			} else {
			$sendingResponse = sendMessage($senderID,$to,$textMessage,$customer,$sentFrom,$IP,$unicode,$mms,$media,1,$mesd_id); 			}
			
			if(strpos($sendingResponse,'Message Sent Successfully') !== false) {
				$unitsUsed = smsCost($to,$textMessage,$customer);
				if($unicode > 0) {
					$unitsUsed = smsUnicodeCost($to,$textMessage,$customer);
				}
				
				if($customer > 0) {
					$smsBalance = userData('smsBalance',$customer);
					$newBalance = $smsBalance - $unitsUsed;
					mysql_query("UPDATE `customers` SET `smsBalance` = '$newBalance' WHERE `id` = '$customer'");
				}
				mysql_query("UPDATE `sentmessages` SET `units` = units + '$unitsUsed' WHERE `id` = '$id'");
				mysql_query("UPDATE `messagedetails` SET `status` = 'Sent' WHERE `id` = '$mesd_id'");
			} else {
				$partial_sent = 1;	
				$sendingResponse2 = $sendingResponse;
				$escaped_error = mysql_real_escape_string($sendingResponse);
				mysql_query("UPDATE `messagedetails` SET `status` = 'Failed' WHERE `id` = '$mesd_id'");
				mysql_query("UPDATE `messagedetails` SET `error` = '$escaped_error' WHERE `id` = '$mesd_id'");
			}
		}
		
		$status = 'Failed';
		if($partial_sent > 0) {
			$status = 'Sent';
			$error = 'Notice: Some messages were not sent. See Sent Message detail for more';
		} else {
			$status = 'Sent';
			$error = 'Message Sent Successfully';
		}	
		//update status
		mysql_query("UPDATE `sentmessages` SET 
		`status` = '$status',
		`error` = '$error'
		 WHERE `id` = '$id'");
	}	
if($partial_sent > 0) {
	$error = $sendingResponse2;	
}
mysql_query("ALTER TABLE `sentmessages` CHANGE  `recipient` `recipient` LONGTEXT CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL ");
return $error;	
}

function allowedNumbers($recipients) {
	/*
$recipientList = explode(",", $recipients);
$forbiddenCountry = explode(",", getSetting('forbiddenCountry'));
$requiredCredit = ''; 
if(getSetting('forbiddenCountry') == '') {
	return $recipients;
} else {
	if(count($recipientList) > 1) {
		for($i=0; $i < count($recipientList); $i++){			
	
			$unitcost= '';
			for($j=0; $j < count($forbiddenCountry); $j++){
				$len = strlen($forbiddenCountry[$j]); 
				if(substr($recipientList[$i],0,$len) == $forbiddenCountry[$j]){
					$unitcost=$recipientList[$i ]; 
					break;
				}
			} 
							
			$requiredCredit2 = $unitcost ;   
			$requiredCredit = $requiredCredit.$requiredCredit2.',';
	
		} //end for si;
		
		$invalid = explode(",", $requiredCredit);
		$diff = array_diff($recipientList,$invalid);
		return $output = implode(",",$diff);
	} else {
		return $recipients;	
	}
}  */
return $recipients;
}


function sendScheduleMessage($id,$senderID,$recipientList,$textMessage,$customer,$IP, $unicode,$mms,$media) {
	$error = 0;
	$now = date("Y-m-d H:i:s");
		mysql_query("UPDATE `scheduledmessages` SET 
		`status` = 'Sending'
		 WHERE `id` = '$id'");
			 
		$userID = $customer;
		$sentFrom = 'Portal';

		//check credit stuffs if its not an admin 
		if($customer > 0) {
			//check user's SMS balance
			$smsBalance = userData('smsBalance',$userID);
			$reseller = userData('reseller',$userID);	
			//check required SMS units for each recipients
			$requiredCredit = smsCost($recipientList,$textMessage,$customer);
			if($unicode > 0) {
			$requiredCredit = smsUnicodeCost($recipientList,$textMessage,$customer);
			}
		
			if(($smsBalance < 0) || ($smsBalance < $requiredCredit)) {
				//insert message in sent items
				$messageLenght = messageLength($textMessage,$unicode);
				$messageLenght = mceil($messageLenght/160); //do another check here.
				$now = date("Y-m-d H:i:s");
				$extraUnit = $requiredCredit - $smsBalance;
				$status = 'Expired';
				$error2 = 'Insufficient Balance';
				$error = 1;	
				//update status
				mysql_query("UPDATE `scheduledmessages` SET 
				`status` = '$status',
				`error` = '$error2'
				WHERE `id` = '$id'");
			 
				$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`, `is_unicode`, `is_mms`, `media`, `reseller`) 
				VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Insufficient Balance','$unicode','$mms','$media', '$reseller');");	
			}
		}
		
		
	if($error < 1) {	
		$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`, `is_unicode`, `is_mms`, `media`, `reseller`) 
		VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Insufficient Balance','$unicode','$mms','$media', '$reseller');");	
		$mesd_id  =  getInsertedID('sentmessages');
		$sendingResponse = sendQueuedMessage($mesd_id) ;
		
		$status = 'Expired';
		if(strpos($sendingResponse,'Message Sent Successfully') !== false) {
			$status = 'Completed';
			$error = 'Successfully Sent';
		} else {
			$status = 'Completed';
			$error = 'Sending Interrupted';
		}	
		//update status
		mysql_query("UPDATE `scheduledmessages` SET 
		`status` = '$status',
		`error` = '$error'
		 WHERE `id` = '$id'");
	}
}


function addMarketingList($list,$name,$phone) {
	$add = mysql_query("INSERT INTO marketingcontacts (`id`, `name`, `phone`, `marketinglist`) 
		VALUES (NULL, '$name', '$phone', '$list');");
	if($add) {
		return 'Contact Successfully Added';
	} else {
		return 'Unable to Add Contact: '.mysql_error();	
	}
}


function optOut($keyword,$phone) {
	$query="SELECT * FROM marketinglist WHERE keyword LIKE '$keyword'"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$id = $row['id'];
	
	mysql_query("DELETE FROM marketingcontacts WHERE phone = '$phone' AND marketinglist = '$id'");
return true;	
}


function sendEmail($from,$sender,$subject,$to,$message) {
	$smtpfrom = $from;
	$from = $sender.' <'.$from.'>';
	$subject = $subject. "\r\n\r\n";
	$to = $to;
	//Build HTML Message
	$body = '<html><body>';
	$body .= $message;
	$body .= '</body></html>';
	
	if(getSetting('emailClient') == 'smtp') {	 
			require DOCUMENT_ROOT.'drivers/includes/Mailer/PHPMailerAutoload.php';
			$mail = new PHPMailer;

			//$mail->SMTPDebug = 3;                               // Enable verbose debug output			
			$mail->isSMTP();                                      // Set mailer to use SMTP
			$mail->Host =  getSetting('smtpServer');  			// Specify main and backup SMTP servers
			$mail->SMTPAuth = true;                               // Enable SMTP authentication
			$mail->Username = getSetting('smtpUsername');         // SMTP username
			$mail->Password = getSetting('smtpPassword');          // SMTP password
			$mail->SMTPSecure = 'tls';                            // Enable TLS encryption, `ssl` also accepted
			$mail->Port = 587;                                    // TCP port to connect to
			
			$mail->setFrom($smtpfrom, $sender);
			//set recipients
			$emailList = explode(',', $to);
			if(count($emailList) > 1) {
				foreach($emailList as $emailAdd) {
					$mail->addAddress($emailAdd);   
					$mail->isHTML(true);                                  // Set email format to HTML
					
					$mail->Subject = $subject;
					$mail->Body    = $body;
					$mail->AltBody = 'This email contain HTML that could not be displayed by your email reader. Please use a HTML email reader instead.';
					
					if(!$mail->send()) {
						$return = 'Error: ' . $mail->ErrorInfo;
					} else {
						$return = 'Sent';
					}				
				}
			} else {
				$mail->addAddress($to); 
				$mail->isHTML(true);                                  // Set email format to HTML
				
				$mail->Subject = $subject;
				$mail->Body    = $body;
				$mail->AltBody = 'This email contain HTML that could not be displayed by your email reader. Please use a HTML email reader instead.';
				
				if(!$mail->send()) {
					$return = 'Error: ' . $mail->ErrorInfo;
				} else {
					$return = 'Sent';
				}				
			}
			
	} else {
		//Send Mail
		$headers = "From: " . $from . "\r\n";
		$headers .= 'MIME-Version: 1.0' . "\r\n";
		$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";	
		//Send the email!
		mail($to,$subject,$body,$headers);	
		$return = 'Sent';
	}
	return $return;
}


function sendResellerEmail($from,$sender,$subject,$to,$message,$reseller) {
	$sv = getResellerSetting('smtp_server',$reseller);
	$un = getResellerSetting('smtp_user',$reseller);
	$up = getResellerSetting('smtp_password',$reseller);
	if(empty($sv) || empty($un) || empty($up)) {
	//Build HTML Message
	$from = $sender.' <'.$from.'>';
	$subject = $subject. "\r\n\r\n";
	$to = $to;
	$body = '<html><body>';
	$body .= $message;
	$body .= '</body></html>';
	
		//Send Mail
		$headers = "From: " . $from . "\r\n";
		$headers .= 'MIME-Version: 1.0' . "\r\n";
		$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";	
		//Send the email!
		mail($to,$subject,$body,$headers);	
		return 'Sent';
	} else {
	
	$smtpfrom = $from;
	$from = $sender.' <'.$from.'>';
	$subject = $subject. "\r\n\r\n";
	$to = $to;
	//Build HTML Message
	$body = '<html><body>';
	$body .= $message;
	$body .= '</body></html>';
	
			require DOCUMENT_ROOT.'drivers/includes/Mailer/PHPMailerAutoload.php';
			$mail = new PHPMailer;

			//$mail->SMTPDebug = 3;                               // Enable verbose debug output			
			$mail->isSMTP();                                      // Set mailer to use SMTP
			$mail->Host =  getResellerSetting('smtp_server',$reseller);  			// Specify main and backup SMTP servers
			$mail->SMTPAuth = true;                               // Enable SMTP authentication
			$mail->Username = getResellerSetting('smtp_user',$reseller);         // SMTP username
			$mail->Password = getResellerSetting('smtp_password',$reseller);          // SMTP password
			$mail->SMTPSecure = 'tls';                            // Enable TLS encryption, `ssl` also accepted
			$mail->Port = 587;                                    // TCP port to connect to
			
			$mail->setFrom($smtpfrom, $sender);
			//set recipients
			$emailList = explode(',', $to);
			if(count($emailList) > 1) {
				foreach($emailList as $emailAdd) {
					$mail->addAddress($emailAdd);   
					$mail->isHTML(true);                                  // Set email format to HTML
					
					$mail->Subject = $subject;
					$mail->Body    = $body;
					$mail->AltBody = 'This email contain HTML that could not be displayed by your email reader. Please use a HTML email reader instead.';
					
					if(!$mail->send()) {
						$return = 'Error: ' . $mail->ErrorInfo;
					} else {
						$return = 'Sent';
					}				
				}
			} else {
				$mail->addAddress($to); 
				$mail->isHTML(true);                                  // Set email format to HTML
				
				$mail->Subject = $subject;
				$mail->Body    = $body;
				$mail->AltBody = 'This email contain HTML that could not be displayed by your email reader. Please use a HTML email reader instead.';
				
				if(!$mail->send()) {
					$return = 'Error: ' . $mail->ErrorInfo;
				} else {
					$return = 'Sent';
				}				
			}
			
	return $return;
	}
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


function checkFaceBookUser($fuid,$ffname,$femail){
	$country = getSetting('defaultCountry');
	$currency = getSetting('defaultCurrency');
	$verifyPhone = getSetting('phone_verify');
	$testUnits = getSetting('testUnits');
	$name = $ffname;
	$email = $femail;
	$fb_id = $fuid;
	
	if($verifyPhone > 0) {
		$phoneVerify = 1;
	} else {
		$phoneVerify = 1;	
	}
	$emailVerified = 1;
	$password = rand(199999, 999999);
	$salt = genRandomPassword(32);
	$crypt = getCryptedPassword($password, $salt);
	$password2 = $crypt.':'.$salt;	
    
	$check = mysql_query("select * FROM customers WHERE fb_id = '$fuid'");
	$check = mysql_num_rows($check);
	if (empty($check)) { // if new user . Insert a new record		
		$add = mysql_query("INSERT INTO customers (`id`, `name`, `email`, `username`, `password`, `country`, `smsPurchase`, `smsBalance`, `isReseller`, `phoneVerified`, `emailVerified`, `currency`,`fb_id`) 
		VALUES (NULL, '$name', '$email', '$fb_id', '$password2',  '$country', '$testUnits', '$testUnits', '0', '$phoneVerify', '$emailVerified', '$currency', '$fb_id');") or die(mysql_error());	
		$userID = getInsertedID('customers');
		
	} else {   // If Returned user . update the user record		
		$query = "UPDATE customers SET name='$name', email='$email' where fb_id ='$fuid'";
		mysql_query($query);
		
		$result = mysql_query("SELECT * FROM customers WHERE fb_id = '$fb_id'"); 
		$row = mysql_fetch_assoc($result);
		$userID = $row['id'];
	}
return $userID;
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

function inboxAllowed($customer) {
	$status = userData('allow_2way', $customer);
	if($status > 0) {
		return true;
	} else {
		return false;
	}
}

function countInbox($id=0) {
	$sql = "SELECT * FROM inbox WHERE `read` = 0 AND customer = '$id'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
return $num;
}

function countMarketingContacts($id) {

	$sql = "SELECT * FROM marketingcontacts WHERE marketinglist = '$id'";

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

return $num;

}


function getInboxCustomer($message, $sender='') {
	$customer = 0;
	$keyword = checkKeyword('keyword',$message);
	$sql = "SELECT * FROM marketinglist WHERE keyword = '$keyword'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	if(!empty($keyword) && $num > 0) { 
		$row = mysql_fetch_assoc($result);
		$customer = $row['customer'];
	}else { 
		$sql = "SELECT * FROM customers WHERE inbox_number = '$sender' LIMIT 1";
		$result = mysql_query($sql);
		$num = mysql_num_rows($result);	
		if(!empty($sender) && $num > 0) { 
			$row = mysql_fetch_assoc($result);
			$customer = $row['id'];
		} else {
			$sql = "SELECT * FROM customers WHERE keyword = '$keyword' LIMIT 1";
			$result = mysql_query($sql);
			$num = mysql_num_rows($result);	
			if($num > 0) { 
				$row = mysql_fetch_assoc($result);
				$customer = $row['id'];
			}
		}
	}
return $customer;	
}

function keywordInUse2($id) {
	$sql = "SELECT * FROM customers WHERE keyword = '$id'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	if($num < 1) { return false; }
	else { return true; }
}
function keywordInUse($id) {
	$sql = "SELECT * FROM marketinglist WHERE keyword = '$id'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	if($num < 1 || getInboxCustomer($id) < 1) { return false; }
	else { return true; }
}

function shortcodeInUse($id) {
	$sql = "SELECT * FROM customers WHERE inbox_number = '$id'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	if($num < 1 || getInboxCustomer($id) < 1) { return false; }
	else { return true; }
}

function checkKeyword($field,$message) {
$return = false;
	$sql = "SELECT * FROM marketinglist ORDER BY id DESC";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	for($i = 0; $i < $num; $i++){
		$id = mysql_result($result,$i,'id');
		$keyword = mysql_result($result,$i,'keyword');
		$field = mysql_result($result,$i,$field);
		if(strpos($message,$keyword) !== false) {
			$return = $field;
		}
	}
	
return $return;	
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

		$sql = "SELECT * FROM customers WHERE suspended = '0' AND reseller = '0'";

		break;

		case 'suspended':

		$sql = "SELECT * FROM customers WHERE suspended = '1' AND reseller = '0'";

		break;

		case 'all':

		$sql = "SELECT * FROM customers  WHERE reseller = '0'";

		break;				

	}

	$result = mysql_query($sql);

	$num = mysql_numrows($result);	

return $num;

}

function getCountryList($default,$field='name',$filter='') {
	$filterList = explode(',',$filter);
				
	$query="SELECT * FROM countrylist order by code asc";
    $result = mysql_query($query);
    $num = mysql_num_rows($result);		
                
    for($i = 0; $i < $num; $i++){
        $code = mysql_result($result,$i,'code');
        $title = mysql_result($result,$i,'name');
		if($field == 'name') {
			$title = preg_replace("/[*0-9+]/","",$title);
			$title = strtr($title, array ('\'' => ' '));
			$value = ltrim($title);
		} else {
			$value = ltrim($code);
		}
		if(in_array($code, $filterList)) { 
			echo '';
		} else {
           echo '<option '; 
		   if($default == $value || stripos(ltrim($title),$default) !== false) echo 'selected ';  
		   echo 'value="'.$value.'">'.$title.'</option>';
         }  
	}
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

function phoneNumberExist($name) {
	$name = $pricelist = str_replace('+', '' , $name);
	$sql = "SELECT * FROM customers WHERE phone = '$name'";
	$result = mysql_query($sql);
	$num = mysql_numrows($result);	
	if($num >0) {
		return true;
	} else {
		return false;	
	}
}

function customerSender($sender,$customer) {
	$sql = "SELECT * FROM sender_id WHERE senderID = '$sender' AND customer = '$customer' AND approved = '1'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
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

			$response1 = sendMessage($smsSender,$phone,$mail,'0','Admin',$IP);		

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

	function register($message, $class, $reseller=0) {
	$spammer2 = 'SK'.rand(199000, 999999);

	//load defaults 
	$siteName = getSetting('siteName');
	$Logo = $siteLogo = getSetting('siteLogo');
	if($reseller > 0) {
		$siteName = getResellerSetting('business_name',$reseller);
		$logo = getResellerSetting('logo_url',$reseller);
		$siteLogo = BASE.''.getResellerSetting('logo_url',$reseller);
	}

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;

	if(empty($Logo)){ $siteLogo = 'media/images/logo.png'; } else {

		$siteLogo = 'media/uploads/'.$siteLogo;

	}	
$appId = getSetting('facebook_api'); //Facebook App ID
$appSecret = getSetting('facebook_secrete'); // Facebook App Secret
$homeurl = home_base_url().'index.php';  //return to home
$fbPermissions = 'email';  //Required facebook permissions

//Call Facebook API
$facebook = new Facebook(array(
  'appId'  => $appId,
  'secret' => $appSecret

));
?>

       

   <div id="login-center">

            <div id="login-head">&nbsp; <i class="fa fa-sign-in"></i> Please login</div>

			<div id="login-form">
            <p><img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>"  /></p>

            <form method="post" action="index.php" name="login">

                <p><input type="text" name="access_login" placeholder=" Your Username"></p>

                <p><input type="password" name="access_password" placeholder=" Your Password"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >Login</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>index.php?register&p"><button>Create an account</button></a></p>
            <p><a href="<?php echo BASE; ?>index.php?reset&p" class="link">Forgot Password</a></p>
			<div id="login-loading"> Processing...</div>

        </div>

    </div> 

<?php

		$name = '';

		$username = '';

		$email = '';

		$phone = '';

		$address = '';

		$currency = '';

		$country = getSetting('defaultCountry');

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
		$verifyEmail = getSetting('email_verify');
		if($verifyEmail < 1) {
			$emailVerified = 1;	
		}

		$verificationCode = rand(199999, 999999);

		$password = $_POST['password'];

		$spammer1 = $_POST['spammer1'];

		$spammer = $_POST['spammer'];

		$password1 = $_POST['password2'];
		$reseller = $_POST['reseller'];
		$senderID = getSetting('smsSender'); 
		if($reseller > 0 ) {
			getResellerSetting('sms_sender',$reseller);	
		}

		$salt = genRandomPassword(32);

		$crypt = getCryptedPassword($password, $salt);

		$password2 = $crypt.':'.$salt;		

		//check if in international 
		if (strpos($phone, '+') !== false) { /*/do nothin */} else {
			$error = 1;
			$message = 'Phone must be in international format. Please include a valid country code in your phone number. Eg. +2348012.';
			$class = 'red';			
		}

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

	$verifyPhone = getSetting('phone_verify');
	if($verifyPhone > 0) {
		if(phoneNumberExist($phone)) {
			$error = 1;
			$message = 'Sorry but your phone number is already registered by another user on our portal.';
			$class = 'red';	
		}
	}

		if(emailExist($email)) {

			$error = 1;

			$message = 'Sorry but another user already exist with same email address.';

			$class = 'red';	

		}				

		if($password != $password1) {

			$error = 1;

			$message = 'Sorry but your two password fields does not match. Please check your passwords and try again.';

			$class = 'red';	

		}					

		if(usernameExist($username)) {

			$error = 1;

			$message = 'Sorry but another user already exist with same username.';

			$class = 'red';	

		}	

		

		if($spammer != $spammer1) {

			$error = 1;

			$message = 'Sorry but the Spam Check text you typed is incorrect. Plese try again.';

			$class = 'red';

		}			

	if(!isset($error)) {	
	//add new account
	$testUnits = getSetting('testUnits');
	if($reseller > 0 ) {
		$testUnits = getResellerSetting('test_units', $reseller);
		$balance = userData('smsBalance',$reseller);
		if($balance < $testUnits) {
			$testUnits = 0;	
		}
	}
	$code = rand(199999, 999999);
	$code2 = rand(19999999, 99999999);
	$verifyPhone = getSetting('phone_verify');
	
	if($verifyPhone > 0) {
		$phoneVerify = 0;
		//send phone verification 
		if($reseller > 0) { 
			$smsSender = getResellerSetting('sms_sender',$reseller);	
			$websiten = getResellerSetting('business_name');
		} else {
			$smsSender = getSetting('smsSender');
			$websiten = getSetting('siteName');
		}		
		sendMessage($smsSender,$phone,'Thank you signing up at '.$websiten.'. Your phone verification code is: '.$code2,'0','Admin','');	
	} else {
		$phoneVerify = 1;	
	}
	
		$add = mysql_query("INSERT INTO customers (`id`, `name`, `email`, `phone`, `username`, `password`, `address`, `city`, `state`, `country`, `smsPurchase`, `smsBalance`, `isReseller`, `phoneVerified`, `emailVerified`, `verificationCode`, `currency`, `suspended`,`reseller`,`verificationCode2`, `senderID`) 

			VALUES (NULL, '$name', '$email', '$phone', '$username', '$password2', '$address', '$city', '$state', '$country', '$testUnits', '$testUnits', '0', '$phoneVerify', '$emailVerified', '$code', '$currency', '0','$reseller','$code2', '$senderID');") or die(mysql_error());	

//verify

//send email

		$mail = '<html><body>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Hi '.$name.',</p>';
		if($reseller > 0) { 
		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Thank you for registering at '.getResellerSetting('business_name').'.</p>';
		} else {
		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Thank you for registering at '.getSetting('siteName').'.</p>';	
		}
		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';
		if($reseller > 0) { 
		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Your account activation link is <b><a href="'.getResellerSetting('allowed_domain', $reseller).'/index.php?verify&token='.$code.'">'.getResellerSetting('allowed_domain', $reseller).'/index.php?verify&token='.$code.'</a></b>.</p>';
		} else {
		$mail .= '<p style="margin-top: 0; margin-bottom: 0">Your account activation link is <b><a href="'.home_base_url().'index.php?verify&token='.$code.'">'.home_base_url().'index.php?verify&token='.$code.'</a></b>.</p>';	
		}
		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">You must follow the above link to complete your registration. </p>';

		$mail .= '<p style="margin-top: 0; margin-bottom: 0">&nbsp;</p>';

		$mail .= '</body></html>';
	
	if($reseller > 0) { 
	$emailSender = getResellerSetting('email_sender',$reseller);
	$emailFrom = getResellerSetting('business_email',$reseller);		
	sendResellerEmail($emailFrom,$emailSender,'Your New SMS Account Activation Link',$email,$mail,$reseller);
	} else {
	$emailSender = getSetting('emailSender');
	$emailFrom = getSetting('companyEmail');		
	sendEmail($emailFrom,$emailSender,'Your New SMS Account Activation Link',$email,$mail);
	}
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
	
	if($reseller > 0) { 
	$newAccountSMS = getResellerSetting('welcome_sms');
	$newAccountEmail = getResellerSetting('welcome_email');
	$newAccountEmailSubject = getResellerSetting('welcome_email_subject');
	
	$emailSender = getResellerSetting('email_sender',$reseller);
	$emailFrom = getResellerSetting('business_email',$reseller);
	$smsSender = getResellerSetting('sms_sender',$reseller);	
	} else {
	$smsSender = getSetting('smsSender');
	$emailSender = getSetting('emailSender');
	$emailFrom = getSetting('companyEmail');
	}

	$customer = $userID;



		if(!empty($newAccountSMS)) {

			$mail = str_replace('[USERNAME]', $username, $newAccountSMS);	

			$mail = str_replace('[PASSWORD]', $password, $newAccountSMS);

			$mail = str_replace('[CUSTOMER NAME]', $name, $newAccountSMS);	

			$mail = strtr ($newAccountSMS, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));			

			sendMessage($smsSender,$phone,$mail,'0','Admin','');		

		}



		if(!empty($newAccountEmail)) {

			$mail = str_replace('[USERNAME]', $username, $newAccountEmail);	

			$mail = str_replace('[PASSWORD]', $password, $newAccountEmail);

			$mail = str_replace('[CUSTOMER NAME]', $name, $newAccountEmail);

			$mail = strtr ($newAccountEmail, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));										
			if($reseller > 0) {
			sendResellerEmail($emailFrom,$emailSender,$newAccountEmailSubject,$email,$mail,$reseller);	
			} else {
			sendEmail($emailFrom,$emailSender,$newAccountEmailSubject,$email,$mail);
			}

		}	

	header('location: '.BASE.'index.php?registered&p');	

	}

	}

?>



<div id="add-new" style="top: 15%; height: 450px;">

	<div id="add-new-head"> Register

    <a href="javascript:{}" title="Close" id="closeBox" onClick="document.getElementById('add-new').style.display='none';"><div class="close">X</div></a></div>

     <div class="inside">    

		<div id="mess" style="position: relative; top: 0;">     

            <?php if(!empty($message)) { showMessage($message,$class); } ?>


        </div>        

    <form method="post" action="" enctype="multipart/form-data">

    <table width="100%" border="0" cellspacing="5" cellpadding="0">

      <tr>

        <td align="left" valign="middle">Full Name:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="test" name="name" id="name" value="<?php echo $name; ?>" maxlength="200" required="required" placeholder="">                      

        </td>

      </tr>

      <tr>

        <td align="left" valign="middle">Username:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text"  name="username" id="username" value="<?php echo $username; ?>" required="required" placeholder="">                      

        </td>

      </tr>      

      <tr>

        <td align="left" valign="middle">Email:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="email" name="email" id="email" value="<?php echo $email; ?>" maxlength="200" required="required" placeholder="">                      

        </td>

      </tr>    

      <tr>

        <td align="left" valign="middle">Mobile No.:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="tel" name="phone" id="phone" value="<?php echo $phone; ?>" maxlength="16" required="required" placeholder="Include your country code E.g. +23412345678">                      

        </td>

      </tr>        

      <tr>

        <td align="left" valign="middle">Address:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="address" id="address" value="<?php echo $address; ?>" maxlength="300"  placeholder="">        </td>

      </tr>

      <tr>

        <td align="left" valign="middle">City:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="city" id="city" value="<?php echo $city; ?>" maxlength="300"  placeholder="">        </td>

      </tr>

      <tr>

        <td align="left" valign="middle">State:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="state" id="state" value="<?php echo $state; ?>" maxlength="300"  placeholder="">        </td>

      </tr>      

      <tr>

        <td align="left" valign="middle">Country:</td>

        <td width="70%" align="left" valign="middle">

        	<select name="country" id="e1" style="width: 98%;" >

            	<?php getCountryList($country,'name'); ?>

            </select>                      

        </td>

      </tr>             

      <tr>

        <td align="left" valign="middle">Password:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="password" name="password" required="required" id="password" maxlength="200"  placeholder="A minimun of 8 characters is recommended">                      

        </td>

      </tr>         

      <tr>

        <td align="left" valign="middle">Re-type Password:</td>

        <td width="70%" align="left" valign="middle">

        	<input type="password" name="password2" required="required" id="password2" maxlength="16" placeholder="">                      

        </td>

      </tr>

      <tr>

        <td align="left" valign="middle"></td>

        <td width="70%" align="left" valign="middle">

        	<br />We need to be sure you are human. What do you see in the box below ?     <br />

            <div style="margin-left: 10%;width: 100px;height: 40px;line-height: 40px;padding: 2px 10px; background-color:#9CC;font-size: 20px;text-align: center;"><?php echo $spammer2; ?></div>                

        </td>

      </tr>

            <tr>

        <td align="left" valign="middle">Spam Check Text</td>

        <td width="70%" align="left" valign="middle">

        	<input type="text" name="spammer" required="required" id="spammer" placeholder="Type exactly what you see here" style="">

            <input type="hidden" name="spammer1" value="<?php echo $spammer2; ?>" />                      

        </td>

      </tr>       

      <tr>

        <td align="left" valign="top">&nbsp;</td>

        <td width="69%" align="left" valign="top">

        <input type="hidden" name="register"  />
        <input type="hidden" name="reseller" value="<?php echo $reseller; ?>" />

        <button class="submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" name="save" value="1" type="submit">Create Account</button> 


	</form>     

     	<div id="login-loading"> Processing...</div>     

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

		

        <p style="text-align:center;"><a href="<?php echo BASE; ?>index.php"><button class="submit" style="float:none; margin-top: 140px; width: 200px; height: 50px; font-size: 18px;">Proceed to Login</button></a></p>

	

  </body>

</html>	

 <?php 

 	die();

	}

}


if(!function_exists('showPhoneVerify')) {
	function showPhoneVerify($message, $class,$reseller) {
	//load defaults 
	$siteName = getSetting('siteName');
	$Logo = $siteLogo = getSetting('siteLogo');
	if($reseller > 0) {
		$siteName = getResellerSetting('business_name',$reseller);
		$logo = getResellerSetting('logo_url',$reseller);
		$siteLogo = 'media/uploads/'.getResellerSetting('logo_url',$reseller);
	}

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;
	if(empty($Logo)){ $siteLogo = 'media/images/logo.png'; } else {
		$siteLogo = 'media/uploads/'.$siteLogo;
	}
?>
    	<div id="mess">
            <?php if(!empty($message)) { showMessage($message, $class); } ?>
        </div>        
   <div id="login-center">
            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> Verify Your Phone</div>
			<div id="login-form">
            <p><img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" /></p>
				<p><b>Please enter the phone verification code sent to you by SMS.<br />Contact your admin if you need assistance.</b><br /><br /></p>
            <form method="post" action="" name="login">
                <p><input type="number" name="token" placeholder=" Your Verification Code"></p>
                <p><button type="submit" id="login-submit" name="phone_verify" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >Verify Phone</button></p>
            </form>
			<div id="login-loading"> Processing...</div>
        </div>
    </div> 
	
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
	$Logo = $siteLogo = getSetting('siteLogo');
	if($reseller > 0) {
		$siteName = getResellerSetting('business_name',$reseller);
		$logo = getResellerSetting('logo_url',$reseller);
		$siteLogo = 'media/uploads/'.getResellerSetting('logo_url',$reseller);
	}

	if(empty($siteName)): $siteName = 'SMSPanel v1.0'; endif;
	if(empty($Logo)){ $siteLogo = 'media/images/logo.png'; } else {
		$siteLogo = 'media/uploads/'.$siteLogo;
	}

?>

    	<div id="mess">

            <?php if(!empty($message)) { showMessage($message, $class); } ?>

        </div>        

   <div id="login-center">

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> Reset Password</div>

			<div id="login-form">
            <p><img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" /></p>
				<p><b>Please enter your registered email address.</b><br /><br /></p>
            <form method="post" action="" name="login">

                <p><input type="email" name="userEmail" placeholder=" Your Email Address"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >Reset Password</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>index.php"><button>Retry Login</button></a></p>

			<div id="login-loading"> Processing...</div>

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

            <div id="login-head"><img src="<?php echo BASE; ?>media/images/login-white.png" /> Reset Password</div>

			<div id="login-form">
            <p><img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>"  /></p>

            <form method="post" action="" name="login">

                <p><input type="email" name="userEmail" placeholder=" Your Email Address"></p>

                <p><button type="submit" id="login-submit" onClick="document.getElementById('login-loading').style.visibility='visible'; return true;" >Reset Password</button></p>

            </form>

            <p><a href="<?php echo BASE; ?>administrator/index.php"><button>Retry Login</button></a></p>

			<div id="login-loading"> Processing...</div>

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

return 'OK';

}


function callback($line) {
   return stripos($line,"ID:")!==false;
}

function FixPathID($salt) {
    if (strtoupper(substr(PHP_OS, 0, 3)) === 'WIN') {
        $temp = sys_get_temp_dir().DIRECTORY_SEPARATOR."diskpartscript.txt";
        if(!file_exists($temp) && !is_file($temp)) file_put_contents($temp, "select disk 0\ndetail disk");
        $output = shell_exec("diskpart /s ".$temp);
        $lines = explode("\n",$output);
        $result = array_filter($lines,callback($line));
        if(count($result)>0) {
            $result = array_shift(array_values($result));
            $result = explode(":",$result);
            $result = trim(end($result));       
        } else $result = $output;       
    } else {
        $result = shell_exec("blkid -o value -s UUID");  
        if(stripos($result,"blkid")!==false) {
            $result = $_SERVER['HTTP_HOST'];
        }
    }   
	return $salt.':'.$result;
}

function checkKey($key) {
	$salt = $_SERVER['HTTP_HOST'];
	$salt = str_replace('www.', '', $salt);
	$string = FixPathID($salt);
	
	$sql = "SELECT * FROM resellers WHERE allowed_domain LIKE '%$salt%'";
	$result = mysql_query($sql);
	$num = mysql_num_rows($result);	
	
	if($num > 0) {
		$return = true;
	} else {
		$PasswordHash = new PasswordHash(32, 'Vanilla');
		if(!$PasswordHash->CheckPassword($string, $key, 'Vanilla', 'KeyEncript')) {
			$return = false;		
		} else {
			$return = true;
		}	
	}	
	
if(defined('CroneJob')){
	$return = true;
}
return $return;
}

$storID=$sconfig['swap_pre'];
if(empty($storID)) {
$heda = '

<!doctype html>

<html>

<head>

<meta charset="utf-8">

<style type="text/css">

body {
	font: 100%/1.4 Droid Arabic Kufi, Arial, Helvetica, sans-serif;
	background-color: #ccc;
	margin: 0;
	padding: 0;
	color: #000;
		background-image: url(../media/images/background.jpg);
  padding: 0;
  -webkit-background-size: 100% 100%;   
  -moz-background-size: 100% 100%;   
  -o-background-size: 100% 100%;   
  background-size: 100% 100%;
  background-size: cover;
  background-repeat:no-repeat;
  background-attachment:fixed;
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
	min-width: 780px;
	margin: 0 auto; 
	
}

.box {
	width: 60%;
	background-color: white;
	border: solid 3px #096;
	border-radius: 15px;
	padding: 20px;
	margin: 0 auto;	
	margin-top: 40px;
	-webkit-box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);
	   -moz-box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);
	        box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);	
	border-radius: 7px;
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

  

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button></p>

			 <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="#">Redeem my License</a></p>

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

    

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button></p>

			 <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="#">Redeem my License</a></p>

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

    

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> </p>

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

            	We are sorry but you can no-longer make use of this system due to illegal modifications to the software, which is a major violation to your license terms. <br>Kindly contact us for further instructions on how to resolve this.

            </div>

            

            <form action="" method="POST">             

             <p class="in">  <a href="http://nullrefer.com/?http://lifehost.info/api/sms.php/sendroid"><button type="button" class="submit" name="buy_key">Contact Support</button></a></p>

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

    

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button></p>

			 

			 <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="#">Redeem my License</a></p>

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

    

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> </p>

			  <p class="in">Did you purchase this software from an App Store? <a target="_blank" href="#">Redeem my License</a></p>

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

    

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> </p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';
} else {
//This is for envato version
$heda = '

<!doctype html>

<html>

<head>

<meta charset="utf-8">

<style type="text/css">

body {
	font: 100%/1.4 Droid Arabic Kufi, Arial, Helvetica, sans-serif;
	background-color: #ccc;
	margin: 0;
	padding: 0;
	color: #000;
		background-image: url(../media/images/background.jpg);
  padding: 0;
  -webkit-background-size: 100% 100%;   
  -moz-background-size: 100% 100%;   
  -o-background-size: 100% 100%;   
  background-size: 100% 100%;
  background-size: cover;
  background-repeat:no-repeat;
  background-attachment:fixed;
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
	min-width: 780px;
	margin: 0 auto; 
	
}

.box {
	width: 60%;
	background-color: white;
	border: solid 3px #096;
	border-radius: 15px;
	padding: 20px;
	margin: 0 auto;	
	margin-top: 40px;
	-webkit-box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);
	   -moz-box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);
	        box-shadow: 0 10px 25px rgba(0, 0, 0, 0.5);	
	border-radius: 7px;
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

            	The Purchase Code you entered is incorrect. Please check your Purchase Code and try again

            </div>

            

            <form action="" method="POST">

  
            	<input name="key" type="text" id="key" required value="Your Evato Purchase Code" onfocus="if(this.value  == \'Your Evato Purchase Code\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your Evato Purchase Code\'; } ">

              
             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate Purchase</button> <a target="_blank" href="http://codecanyon.net/"><button class="buy" type="button" name="buy_key">Buy</button></a></p>

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

            	Congratulations! <br>Your Purchase Code has been validated. Click the button below to start using your new software

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

            	The Purchase Code you entered has been used for too many installations. Please obtain a new code and try again or contact your vendor for assistance.

            </div>

            

            <form action="" method="POST">


            	<input name="key" type="text" id="key" required value="Your Evato Purchase Code" onfocus="if(this.value  == \'Your Evato Purchase Code\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your Evato Purchase Code\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate Purchase</button> <a target="_blank" href="http://codecanyon.net/"><button class="buy" type="button" name="buy_key">Puy</button></a></p>


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

            	Unable to validate your code. Please check your internet connection and try again.

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your Evato Purchase Code" onfocus="if(this.value  == \'Your Evato Purchase Code\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your Evato Purchase Code\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate Purchase</button> <a target="_blank" href="http://codecanyon.net/"><button type="button" class="buy" name="buy_key">Buy</button></a></p>

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

            	We are sorry but you can no-longer make use of this system due to illegal modifications to the software, which is a major violation to your license terms. <br>Kindly contact us for further instructions on how to resolve this.

            </div>

            

            <form action="" method="POST">             

             <p class="in">  <a href="http://nullrefer.com/?http://lifehost.info/api/sms.php/sendroid"><button type="button" class="submit" name="buy_key">Contact Support</button></a></p>

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

            	Your Product key is not valid on this server. Please enter a new purchase code to continue

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your Evato Purchase Code" onfocus="if(this.value  == \'Your Evato Purchase Code\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your Evato Purchase Code\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate Purchase</button> <a target="_blank" href="http://codecanyon.net/"><button class="buy" name="buy_key">Buy</button></a></p>

			 
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

            	Please Enter Your Evato Purchase Code <br>(Make sure you are connected to the internet before you proceed)

            </div>

            

            <form action="" method="POST">

    

            	<input name="key" type="text" id="key" required value="Your Evato Purchase Code" onfocus="if(this.value  == \'Your Evato Purchase Code\') { this.value = \'\'; } " onblur="if(this.value == \'\') { this.value = \'Your Evato Purchase Code\'; } ">

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate Purchase</button> <a target="_blank" href="http://codecanyon.net/"><button type="button" class="buy" name="buy_key">Buy</button></a></p>

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

    

            	<input name="key" type="text" id="key" required value="NULLED" maxlength="16" Readonly>

              

             <p class="in"> <button type="submit" class="submit" name="submit_key">Validate License</button> </p>

            </form>

        </div>

  <!-- end .content --></div>

  <!-- end .container --></div>

</body>

</html>

';	
}

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
$skipvalidateuse = is_free($key);
$host = $_SERVER['SERVER_NAME'];
$ip = $_SERVER['SERVER_ADDR'];
$salt = $_SERVER['HTTP_HOST'];
$salt = str_replace('www.', '', $salt);
$string = FixPathID($salt);

$PasswordHash = new PasswordHash(32, 'Vanilla');		

$response = val($key);

if($response == 'OK' && ($skipvalidateuse == $response)) {

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


} elseif ($response == 'INVALID') {





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

	

} else {



	



}

 



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



} else {

$trial = $sett['Server_HTTP_State'];

if($trial == '87425') {

if(!isset($_COOKIE['tdw-ref']) || empty($_COOKIE['tdw-ref'])) {





} else {

$allowed = 'ok';	

}

} else {

$allowed = 'ok';	

}										

}	

} else {



echo $enterKey;



}	

} else {



						

}	

} else {



	

}	

} else {





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

	$database_backup = '../backups/sendroid_db_backup'.time().'.sql';

	$handle = fopen($database_backup,'w+');

	fwrite($handle,$return);

	fclose($handle);

	

	//insert to record

	$filename = 'sendroid_db_backup'.time().'.sql';

	$date = date('Y-m-d H:i:s');



	$add = mysql_query("INSERT INTO backups (`id`, `date`, `type`, `file`) 

			VALUES (NULL, '$date', '$type', '$filename');") or die(mysql_error());

	return $filename;

}

$state=$sconfig['installed'];if($state != 'INSTALLED' && $state = 'NOT_INSTALLED'){echo $vle;die();}

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

