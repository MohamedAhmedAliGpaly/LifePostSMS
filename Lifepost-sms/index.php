<?php header('Access-Control-Allow-Origin: *'); ?>

<?php

ob_start();
define('SMSPanel', true);
define('INDEX', 1);
include_once('../drivers/initiator.inc');
$sentFrom = 'API';
$IP = '-';
if(isset($_REQUEST['action'])) {
	$action = $_REQUEST['action'];
} else {
	$action = 'compose';	
}
if(isset($_REQUEST['sendondate'])) {
	$scheduled_date = $_REQUEST['sendondate'];
	$scheduled_date =  str_replace('T', ' ', $scheduled_date);
	if(strtotime($scheduled_date) > strtotime(date('Y-m-d H:i:s')))  {
		$action = 'schedule';
	}
}
if(isset($_REQUEST['mms'])) {
	$mms = $_REQUEST['mms'];
} else {
	$mms = '0';	
}
if(isset($_REQUEST['unicode'])) {
	$unicode = $_REQUEST['unicode'];
} else {
	$unicode = '0';	
}
if(isset($_REQUEST['media'])) {
	$media = $_REQUEST['media'];
} else {
	$media = '';	
}


	if(!isset($_REQUEST['no_login'])) {
		$IP = getenv('REMOTE_ADDR');
			if(!isset($_REQUEST['username']) || empty($_REQUEST['username'])) {
			die('Username Not Supplied');	
		}
	
		if(isset($_REQUEST['password']) && !empty($_REQUEST['password'])) {
			$name = $_REQUEST['username'];
			$password = $_REQUEST['password'];
		  	$query="SELECT * FROM customers WHERE (username LIKE '$name') OR (email = '$name')"; 
			$result = mysql_query($query) or die(mysql_error());  
			$row = mysql_fetch_assoc($result); 
			$num = mysql_num_rows($result);
			if($num < 1) {
		    		die('Invalid User: '.$name);
			}
			$pwsalt = explode( ":",$row["password"]);	
			$pass2 = $row["password"];
			if(md5($password . $pwsalt[1]) != $pwsalt[0] && md5($password) != $row["password"]) {	
    			die('Invalid Password');
  			}
			$customer = $row['id'];	
		} else {	
			if(!isset($_REQUEST['api_key']) || empty($_REQUEST['api_key'])) {
				die('API Key Not Supplied');	
			}	
			$name = $_REQUEST['username'];
			$password = $_REQUEST['api_key'];
	  		$query="SELECT * FROM customers WHERE (username LIKE '$name') OR (email = '$name')"; 
			$result = mysql_query($query) or die(mysql_error());  
			$row = mysql_fetch_assoc($result); 
			$num = mysql_num_rows($result);
			if($num < 1) {
		    		die('Invalid User: '.$name);
			}
			$pwsalt = explode( ":",$row["password"]);	
			$pass2 = $row["password"];
			if($password != $row["password"]) {	
    				die('Invalid API Key');
	  		}
			$customer = $row['id'];	
		} 
	}

	if($action == 'compose' || $action == 'schedule' ) { 
		if(!isset($_REQUEST['message']) || empty($_REQUEST['message'])) {
			die('Message Not Supplied');	
		}
		if(!isset($_REQUEST['sender']) || empty($_REQUEST['sender'])) {
			die('SenderID Not Supplied');	
		}
		if( @$_REQUEST['phonebook'] < 1 && (!isset($_REQUEST['to']) || empty($_REQUEST['to'])) ) {
			die('Recipient Not Supplied');	
		}		
		
		if(getSetting('dynamicSender') == 0) { 
			if($customer > 0) {
				if(!customerSender($_REQUEST['sender'],$customer)) {
					die('Sender ID Not Assigned To Your Account');	
				}
			}
		}
	} 
	if($action == 'compose') {
		//send SMS	
		$senderID = $_REQUEST['sender'];
		$recipientList = $_REQUEST['to'];
		$textMessage = $_REQUEST['message'];
		
		//check for phonebook
		if(isset($_REQUEST['phonebook']) && $_REQUEST['phonebook'] > 0) {
			$phonebook = $_REQUEST['phonebook'];
			$sSQL = "SELECT phone FROM contacts WHERE phonebook = '$phonebook'";
			 $MailtoDelimiter = ",";
				$rphonelist = mysql_query($sSQL);
				$phonelist = '';
				while (list ($phone) = mysql_fetch_row($rphonelist)) {
					$sPhone = $phone;
					if($sPhone) {
						if($phonelist) {
							$phonelist .= $MailtoDelimiter;
						}
						if (!stristr($phonelist, $sPhone)) {
							$phonelist .= $sPhone;
						}
					}
				}
			if(!empty($phonelist)) {	
				$recipientList .= ','.$phonelist; 
			}
		}
		if(!empty($_REQUEST['countryList'])) {
			$recipientList = alterPhone($recipientList,$_REQUEST['countryList']);	
		}
		$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );	
		
			//check user's SMS balance
			$smsBalance = userData('smsBalance',$customer);	
			$reseller = userData('reseller',$customer);			
			//check required SMS units for each recipients
			$requiredCredit = smsCost($recipientList,$textMessage,$customer);
			if($unicode > 0) {
				$requiredCredit = smsUnicodeCost($recipientList,$textMessage,$customer);
			}
			//check if user has enough units	
			if($smsBalance - $requiredCredit < 1) {
				//insert message in sent items
				$messageLenght = strlen($textMessage);
				  $messageLenght = mceil($messageLenght/160);
				  if($unicode > 0) {
					  $messageLenght = mceil($messageLenght/70);
				  }
				  $now = date("Y-m-d H:i:s");
				$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`, `is_mms`, `is_unicode`, `media`, `reseller`) 
				VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$messageLenght', 'Failed', '0', '$sentFrom', '$IP', 'Insufficient Balance', '$mms', '$unicode','$media', '$reseller');");
								
				die('Insufficient Balance');				
			}
			
			//save message for sending 
			$messageLenght = strlen($textMessage);
			$messageLenght = mceil($messageLenght/160);
			  if($unicode > 0) {
				  $messageLenght = mceil($messageLenght/70);
			  }
				  
			 $now = date("Y-m-d H:i:s");
			$add = mysql_query("INSERT INTO sentmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `units`, `sentFrom`, `IP`, `error`, `is_mms`, `is_unicode`, `media`, `reseller`) 
				VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$messageLenght', 'Sending', '0', '$sentFrom', '$IP', '', '$mms', '1','$media', '$reseller');");
			$mesd_id  =  getInsertedID('sentmessages');
				
			//send massage in batches
			$activeGateway = getSetting('activeGateway');	
			$batchSize = setGateway($activeGateway,'batchSize');
			$recipientcount = explode(',',$recipientList);
			
			if(count($recipientcount) <= $batchSize){			
				//send message now
				$sendingResponse = sendQueuedMessage($mesd_id) ;
				
			} else 	{ 	
				$sendingResponse = 'Message Sent Successfully';
			}
			
			if(strpos($sendingResponse,'Message Sent Successfully') !== false) {
				die('Message Sent Successfully');
			} else {
				die('Sending Interupted: '.$sendingResponse);
			}		
	}

	if($action == 'schedule') {
		//schedule SMS	
		$senderID = $_REQUEST['sender'];
		$recipientList = $_REQUEST['to'];
		$textMessage = $_REQUEST['message'];
		
		//check for phonebook
		if(isset($_REQUEST['phonebook']) && $_REQUEST['phonebook'] > 0) {
			$phonebook = $_REQUEST['phonebook'];
			$sSQL = "SELECT phone FROM contacts WHERE phonebook = '$phonebook'";
			 $MailtoDelimiter = ",";
				$rphonelist = mysql_query($sSQL);
				$phonelist = '';
				while (list ($phone) = mysql_fetch_row($rphonelist)) {
					$sPhone = $phone;
					if($sPhone) {
						if($phonelist) {
							$phonelist .= $MailtoDelimiter;
						}
						if (!stristr($phonelist, $sPhone)) {
							$phonelist .= $sPhone;
						}
					}
				}
			if(!empty($phonelist)) {	
				$recipientList .= ','.$phonelist; 
			}
		}
		if(!empty($_REQUEST['countryList'])) {
			$recipientList = alterPhone($recipientList,$_REQUEST['countryList']);	
		}
		$recipientList = $nn = preg_replace("/[^0-9+,]/", "", $recipientList );		
		
		$date = date('Y-m-d H:i:s', strtotime($scheduled_date));
			$smsBalance = userData('smsBalance',$customer);			
			$requiredCredit = smsCost($recipientList,$textMessage,$customer);
			if($unicode > 0) {
				$requiredCredit = smsUnicodeCost($recipientList,$textMessage,$customer);
			}
			if($smsBalance - $requiredCredit < 0) {
				die('Insufficient Balance');				
			}

			$messageLenght = strlen($textMessage);
			$pages = mceil($messageLenght/160);
			$now = date("Y-m-d H:i:s");
			$add = mysql_query("INSERT INTO scheduledmessages (`id`, `message`, `senderID`, `recipient`, `date`, `customer`, `pages`, `status`, `scheduleDate`, `IP`, `is_unicode`) 
			VALUES (NULL, '$textMessage', '$senderID', '$recipientList', '$now', '$customer', '$pages', 'Pending', '$date', '$IP', $unicode);");	
			die('Message Sent Successfully');	
	}	
	
	if($action == 'balance') {
		//check balance	
		$smsBalance = userData('smsBalance',$customer);
		die('Balance: '.$smsBalance);
	}	
	
	if($action == 'marketing_list') {
		$return ='';
		$sql = "SELECT * FROM marketinglist WHERE customer = '$customer' ORDER BY id DESC";
		$result = mysql_query($sql);
		$num = mysql_num_rows($result);	
		if($num < 1) {
			die('No Marketing Lists Found');	
		}
		for($i = 0; $i < $num; $i++){
			$id = mysql_result($result,$i,'id');
			$name = mysql_result($result,$i,'title');
			$return .= $id.':'.$name.',';	
		}
		die($return);
	}
	
	if($action == 'add_to_list') {
		$list_id = $_REQUEST['list_id'];
		$name = $_REQUEST['name'];
		$phone = $_REQUEST['phone'];
		
		if(empty($list_id) || empty($phone)) {
			die( 'Missing Parameters' );	
		}
		
		$sql = "SELECT * FROM marketinglist WHERE customer = '$customer' ORDER BY id DESC";
		$result = mysql_query($sql);
		$num = mysql_num_rows($result);	
		if($num < 1) {
			die('List Not Found');	
		}
		
		$return = addMarketingList($list_id,$name,$phone);
		die($return);
	}

/*  THis set of API were added on Version 4.6.0 
	These are intended for use with the SEndroid Mobile Application
	PLEASE DO NOT MODIFY ANY PART OF THESE CODES
*/

//Authenticate
if($action == 'login') {
	$login = $_REQUEST['username'];
	$name = $_REQUEST['username'];
	$password = $_REQUEST['password'];

  	$query="SELECT * FROM customers WHERE (username LIKE '$name') OR (email = '$name')"; 
	$result = mysql_query($query) or die(mysql_error());  
	$row = mysql_fetch_assoc($result); 
	$num = mysql_num_rows($result);
	if($num < 1) {
		$status = "This account does not exist. Please check your details or sign-up if you are new.";
		$encoded = $status.','.'0';
		$userdata =  json_encode( explode(",",$encoded) );
		die($userdata); 
	}

	$pwsalt = explode( ":",$row["password"]);	
	$pass2 = $row["password"];
	if(md5($password . $pwsalt[1]) != $pwsalt[0] && md5($password) != $row["password"]) {	
	    $status = "Oops! Your username or password is incorrect.";
		$encoded = $status.','.'0';
		$userdata =  json_encode( explode(",",$encoded) );
		die($userdata); 
	}  else {
		$query="SELECT * FROM customers WHERE (email = '$name') OR (username LIKE '$name')"; 
		$result = mysql_query($query) or die(mysql_error());  
		$row = mysql_fetch_assoc($result); 
		$user_id = $row['id'];
		
		if($row['emailVerified'] < 1) {
			$userTimeZone = $row['timeZone'];
			$day = date("Y-m-d H:i:s");
			mysql_query("UPDATE  `customers` SET  `lastLogin` =  '$day' WHERE `id` ='$user_id'");
			$encoded = 'success'.','.$row['id'].','.$row['username'].','.$row['senderID'].','.$row['phoneVerified'].','.$row['password'];
			$userdata =  json_encode( explode(",",$encoded) );
			die($userdata);  
		} elseif($row['suspended'] > 0) {
			$status = "Your account is currently suspended. Please contact our support centre for assistance.";
			$encoded = $status.','.'0';
			$userdata =  json_encode( explode(",",$encoded) );
			die($userdata); 
		} else {
			$phone_verify = getSetting('phone_verify');
			if($row['phoneVerified'] < 1 && $phone_verify > 0) {
				$userTimeZone = $row['timeZone'];
				$day = date("Y-m-d H:i:s");
				mysql_query("UPDATE  `customers` SET  `lastLogin` =  '$day' WHERE `id` ='$user_id'");
				$encoded = 'success'.','.$row['id'].','.$row['username'].','.$row['senderID'].','.$row['phoneVerified'].','.$row['password'];
				$userdata =  json_encode( explode(",",$encoded) );
				die($userdata); 
			} else {	
				$userTimeZone = $row['timeZone'];
				$day = date("Y-m-d H:i:s");
				mysql_query("UPDATE  `customers` SET  `lastLogin` =  '$day' WHERE `id` ='$user_id'");
				$encoded = 'success'.','.$row['id'].','.$row['username'].','.$row['senderID'].','.$row['phoneVerified'].','.$row['password'];
				$userdata =  json_encode( explode(",",$encoded) );
				die($userdata); 
			}
		}
	}
}
//contact
if($action == 'contact_us') {
	$name = $_REQUEST['name'];
	$message = $_REQUEST['message'];
	$email = $_REQUEST['email'];
	$emailSender = getSetting('emailSender');
	$emailFrom = getSetting('companyEmail');		
	sendEmail($emailFrom,$emailSender,'Contact Message From '.$name.' Via Mobile App',$email,$message);	
	die("success");
}

//create
if($action == 'register') {
	$name = $_REQUEST['name'];
	$username = $_REQUEST['username'];
	$email = $_REQUEST['email'];
	$phone = $_REQUEST['phone'];
	$phone = str_replace('+', '', $phone);
	$currency = getSetting('defaultCurrency');
	$country = getSetting('defaultCountry');
	$phoneVerified = '1';
	$emailVerified = '0';
	$verificationCode = rand(199999, 999999);
	$password = $_REQUEST['password'];
	$password1 = $_REQUEST['password2'];
	$senderID = getSetting('smsSender'); 
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
	}

	if($phone == '123456789' || $phone == '12345678' || $phone == '1234567' || $phone == '123456') {
		$error = 1;
		$message = 'Sorry but your phone number is not recorgnized. Please use a valid Moble Number including your country code .';
	}

	$verifyPhone = getSetting('phone_verify');
	if($verifyPhone > 0) {
		if(phoneNumberExist($phone)) {
			$error = 1;
			$message = 'Sorry but your phone number is already registered by another user on our portal.';
		}
	}

	if(emailExist($email)) {
		$error = 1;
		$message = 'Sorry but another user already exist with same email address.';
	}				

	if($password != $password1) {
		$error = 1;
		$message = 'Sorry but your two password fields does not match. Please check your passwords and try again.';
	}					

	if(usernameExist($username)) {
		$error = 1;
		$message = 'Sorry but another user already exist with same username.';
	}	


	if(!isset($error)) {	
		//add new account
		$testUnits = getSetting('testUnits');
		$code = rand(199999, 999999);
		$code2 = rand(19999999, 99999999);
		$verifyPhone = getSetting('phone_verify');
	
		if($verifyPhone > 0) {
			$phoneVerify = 0;
			//send phone verification 	
			$smsSender = getSetting('smsSender');
			$websiten = getSetting('siteName');
			sendMessage($smsSender,$phone,'Thank you signing up at '.$websiten.'. Your phone verification code is: '.$code2,'0','Admin','');	
		} else {
			$phoneVerify = 1;	
		}
	
		$add = mysql_query("INSERT INTO customers (`id`, `name`, `email`, `phone`, `username`, `password`, `address`, `city`, `state`, `country`, `smsPurchase`, `smsBalance`, `isReseller`, `phoneVerified`, `emailVerified`, `verificationCode`, `currency`, `suspended`,`reseller`,`verificationCode2`, `senderID`) 
		VALUES (NULL, '$name', '$email', '$phone', '$username', '$password2', '$address', '$city', '', '$country', '$testUnits', '$testUnits', '0', '$phoneVerify', '$emailVerified', '$code', '$currency', '0','0','$code2', '$senderID');") or die(mysql_error());	

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

		$sql = "SELECT * FROM customers ORDER BY id DEsC LIMIT 1";
		$result = mysql_query($sql);
		$num = mysql_num_rows($result);
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
			$mail = strtr($newAccountSMS, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));			
			sendMessage($smsSender,$phone,$mail,'0','Admin','');		
		}

		if(!empty($newAccountEmail)) {
			$mail = str_replace('[USERNAME]', $username, $newAccountEmail);	
			$mail = str_replace('[PASSWORD]', $password, $newAccountEmail);
			$mail = str_replace('[CUSTOMER NAME]', $name, $newAccountEmail);
			$mail = strtr($newAccountEmail, array('[PASSWORD]'=>$password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));										
			sendEmail($emailFrom,$emailSender,$newAccountEmailSubject,$email,$mail);
		}
		$phone_verify = getSetting('phone_verify');	
		$encoded = 'success'.','.$phone_verify;
		$userdata =  json_encode( explode(",",$encoded) );
		die($userdata);
	} else {
		$encoded = $message.',0';
		$userdata =  json_encode( explode(",",$encoded) );
		die($userdata);
	}
	
}
		
//reset password
if($action == 'reset_password') {
	$email2 = $_REQUEST['email'];	
	$query = "SELECT * FROM customers WHERE email = '$email2'";
	$result = mysql_query($query) or die(mysql_error());
	$num = mysql_num_rows($result);	
	if($num < 1) {
		 die("Oops! This email is not registered on our website.");
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
		$smsSender = getSetting('smsSender');
		$emailSender = getSetting('emailSender');
		$emailFrom = getSetting('companyEmail');
		
		if(!empty($newPasswordSMS)) {
			$mail = str_replace('[USERNAME]', $username, $newPasswordSMS);	
			$mail = str_replace('[PASSWORD]', $password, $newPasswordSMS);
			$mail = str_replace('[CUSTOMER NAME]', $name, $newPasswordSMS);	
			$mail = strtr($newPasswordSMS,array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));																
			sendMessage($smsSender,$phone,$mail,$customer,'Admin','-');		
		}

		if(!empty($newPasswordEmail)) {
			$mail = str_replace('[USERNAME]', $username, $newPasswordEmail);	
			$mail = str_replace('[PASSWORD]', $password, $newPasswordEmail);
			$mail = str_replace('[CUSTOMER NAME]', $name, $newPasswordEmail);
			$mail = strtr ($newPasswordEmail, array ('[PASSWORD]' => $password,'[CUSTOMER NAME]' => $name,'[USERNAME]' => $username));										
			sendEmail($emailFrom,$emailSender,$newPasswordEmailSubject,$email2,$mail);
		}	
		die("success");
	}	
}

//check balance
if($action == 'get_balance') {
	$customer = $_REQUEST['userID'];
	$smsBalance = userData('smsBalance',$customer);
	die($smsBalance);	
}

//messsage history
if($action == 'history') {
	$customer = $_REQUEST['userID'];
	$query="SELECT * FROM `sentmessages` WHERE customer = '$customer' ORDER BY id DESC LIMIT 20";
	$result = mysql_query($query) or die(mysql_error());
	$numP = $numCart = mysql_num_rows($result); 
	 
	if($numP < 1) { 
		$return = ' <ul class="responsive_table">';
		$return .= '<li class="table_row">
            <div class="table_section_14">Date</div>
            <div class="table_section_28">Text</div>
            <div class="table_section_14">Credits</div> 
         </li>'; 
		$return .= '<li class="table_row">
            <div class="table_section_14"></div>
            <div class="table_section_28">No Messages Found!</div>
            <div class="table_section_14"></div> 
         </li></ul>'; 
	} else {
		$return = "<style> green {color: green;	} red{color: red;}</style>";		
		$return .= ' <ul class="responsive_table">';
		$return .= '<li class="table_row">
            <div class="table_section_14">Date</div>
            <div class="table_section_28">Text</div>
            <div class="table_section_14">Credits</div> 
         </li>'; 
		for($i = 0; $i < $numP; $i++) { 	
			$tag1 = '';
			$tag2 = '';
			if(mysql_result($result,$i,'status') == 'Sent') {
				$tag1 = '<green>';
				$tag2 = '<green>';	
			}
			if(mysql_result($result,$i,'status') == 'Failed') {
				$tag1 = '<red>';
				$tag2 = '<red>';	
			}
			$return .= '<li class="table_row">
                 <div class="table_section_14">'.$tag1.date('d/m/Y H:i', strtotime(mysql_result($result,$i,'date'))).$tag2.'</div>
                 <div class="table_section_28">'.$tag1.shorten(mysql_result($result,$i,'message'),50).$tag2.'</div>
                 <div class="table_section_14">'.$tag1.mysql_result($result,$i,'units').$tag2.'</div> 
            </li>'; 
		}
		$return .= '</ul>';
	}
	die( $return );	
}

//get phonebook

if($action == 'phonebook') {
	$customer = $_REQUEST['userID'];
	$sql = "SELECT p . * 
			FROM phonebooks p
			LEFT JOIN phonebook_owners o ON p.id = o.phonebook
			WHERE p.customer = '$customer' OR o.customer = '$customer'";
	$result = mysql_query($sql)or die(mysql_error());
	$num = mysql_num_rows($result);				
	$addresses = '';
	if($num < 1) {
		$addresses .= '<option value="0"> - No phonebooks available - </option>';
	}
	for($i = 0; $i < $num; $i++){
		$id = mysql_result($result,$i,'id');
		$title = mysql_result($result,$i,'title');
		if($i == 0) {$sel='selected';}
             $addresses .= '<option value="'.$id.'"'.' '.$sel.'>'.$title.' ('.countContacts($id).' Contacts) </option>';
	}   
	die($addresses);	
}

if($action == 'countryList') {
	$countryList = "<option value='' > International [Specify country code for each recipient]</option>"; 
    $countryList .= getCountryList(getSetting('defaultCountry'),'code'); 
	die($countryList);  	
}

if($action == 'activate') {
	$code = $_REQUEST['code'];
	$result = mysql_query("SELECT * FROM customers WHERE verificationCode2 = '$code'"); 
	$found = mysql_num_rows($result);	
	
	if($found < 1) {
		die("Oops! Your verification code was invalid. Please enter the correct code or contact admin for assistance.");
	} else {
		$row = mysql_fetch_assoc($result);
		$phoneVerified = $row['phoneVerified'];
		$update = mysql_query("UPDATE customers SET phoneVerified = '1' WHERE verificationCode2 = '$code'");
		die("success");
	}	
}	

//messsage inbox
if($action == 'inbox') {
	$customer = $_REQUEST['userID'];
	$query="SELECT * FROM `inbox` WHERE customer = '$customer' ORDER BY id DESC LIMIT 50";
	$result = mysql_query($query) or die(mysql_error());
	$numP = $numCart = mysql_num_rows($result); 
	 
	if($numP < 1) { 
		$return = ' <ul class="responsive_table">';
		$return .= '<li class="table_row">
            <div class="table_section_14">Date</div>
            <div class="table_section_42">Text</div>
         </li>'; 
		$return .= '<li class="table_row">
            <div class="table_section_14"></div>
            <div class="table_section_42">No Messages Found!</div>
         </li></ul>'; 
	} else {
		$return = "<style> green {color: green;	} red{color: red;}</style>";		
		$return .= ' <ul class="responsive_table">';
		$return .= '<li class="table_row">
            <div class="table_section_14">Date</div>
            <div class="table_section_42">Message</div>
         </li>'; 
		for($i = 0; $i < $numP; $i++) { 	
			$tag1 = '';
			$tag2 = '';
			$return .= '<li class="table_row">
                 <div class="table_section_14">'.$tag1.date('d/m/Y H:i', strtotime(mysql_result($result,$i,'date'))).$tag2.'</div>
                 <div class="table_section_42"><strong>'.$tag1.mysql_result($result,$i,'sender').'</strong><br>'.mysql_result($result,$i,'message').$tag2.'</div>
            </li>'; 
		}
		$return .= '</ul>';
	}
	die( $return );	
}

//messsage history
if($action == 'transaction') {
	$customer = $_REQUEST['userID'];
	$query="SELECT * FROM `transactions` WHERE customer = '$customer' ORDER BY id DESC LIMIT 50";
	$result = mysql_query($query) or die(mysql_error());
	$numP = $numCart = mysql_num_rows($result); 
	 
	if($numP < 1) { 
		$return = ' <ul class="responsive_table">';
		$return .= '<li class="table_row">
            <div class="table_section_14">ID</div>
			<div class="table_section_14">Date</div>
			<div class="table_section_14">Amount</div>
            <div class="table_section_14">Credits</div
         </li>'; 
		$return .= '<li class="table_row">
            <div class="table_section_14"></div>
            <div class="table_section_28">No Records Found!</div>
            <div class="table_section_14"></div> 
         </li></ul>'; 
	} else {
		$return = "<style> green {color: green;	} red{color: red;}</style>";		
		$return .= ' <ul class="responsive_table">';
		$return .= '<li class="table_row">
            <div class="table_section_14">ID</div>
			<div class="table_section_14">Date</div>
			<div class="table_section_14">Amount</div>
            <div class="table_section_14">Credits</div> 
         </li>'; 
		for($i = 0; $i < $numP; $i++) { 	
			$tag1 = '';
			$tag2 = '';
			if(mysql_result($result,$i,'status') == '3') {
				$tag1 = '<green>';
				$tag2 = '<green>';	
			}
			if(mysql_result($result,$i,'status') == '4') {
				$tag1 = '<red>';
				$tag2 = '<red>';	
			}
			$return .= '<li class="table_row">
                 <div class="table_section_14">'.$tag1.mysql_result($result,$i,'id').$tag2.'</div>
				 <div class="table_section_14">'.$tag1.date('d/m/Y H:i', strtotime(mysql_result($result,$i,'date'))).$tag2.'</div>
				 <div class="table_section_14">'.$tag1.mysql_result($result,$i,'cost').$tag2.'</div>
                 <div class="table_section_14">'.$tag1.mysql_result($result,$i,'units').$tag2.'</div> 
            </li>'; 
		}
		$return .= '</ul>';
	}
	die( $return );	
}
?>
