<?php defined('SMSPanel') or die('Snooping around is not allowed. Please use the front door');


/* 
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Start Creating your custom functions and plugins below this line 

====== IMPORTANT NOTICE TO DEVELOPERS ==============
1, Always check if function/class names already exist before you define new once
2, Do bear in mind that errors in your custom scripts or plugins can affect the performance of the systems
3, If you notice a bug after you make changes to this file, we recommend you temporally disable your changes until you are able to debug your code.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

//   This is a sample function to retrieve gateway ID of sent messages and update in database.
//   This example was implemented for SMSKit gateway
//   This ID can be used to implement delivery report .

if(!function_exists('get_message_id')) {
	function get_message_id($gateway_response,$local_id) {		
	/*  //Remove this line to use
	
		$local_id = $local_id; // THis is the ID of the message to update in Local Database passed by the system
		
		 //  Retrieve success response from the SMS gateway API.
		 //  This is alread passed in the $gateway_response variable by the system on successful submission of
		 //  message to the gateway API
		 
		 $success_response_stringe = $gateway_response;
		 
		//extract message ID
		$responseBody = strtr ($success_response_stringe, array (' ' => ''));	  //remove empty space
		$responseBody = explode(':',$responseBody);   // Build an array from the ":" separated response
		$gatewayID = $responseBody[1];     //This is the gateway Message ID returned by the gateway API
											//You can use this ID to implement DLR if you wish
					
		//update the gateway ID on the sent message details table
		 mysql_query("UPDATE `messagedetails` SET `gateway_id` = '$gatewayID' WHERE `id` = '$local_id'"); 	 
	
	
	*/ //Remove this line to use 
	}
}
?>

