<?
include_once('./v.php');
 $timeout = 200;
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL,"http://lifebill.life-host.info/version/version-lifepostsms.php");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);
    $curlresult=curl_exec ($ch);
if(curl_getinfo($ch, CURLINFO_HTTP_CODE) === 200 )
{
                if (preg_match("/$version/i", $curlresult)) {

    } else {
                          echo '<meta http-equiv="refresh" content="1;url=../closeupdate.php"/>';
                         

    }  
}elseif(curl_getinfo($ch, CURLINFO_HTTP_CODE) === 404 ){
  //echo 'Mohamedgpaly2';
}else{
   //echo 'Mohamedgpaly3';
}

?>
<?php defined('SMSPanel') or die('Snooping around is not allowed. Please use the front door'); $message='';?>
<?php if(!isset($_REQUEST['csv'])) { ?>
<!DOCTYPE html>
<html PUBLIC "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd" dir="rtl">
<head>
<meta http-equiv="Page-Enter" CONTENT="RevealTrans(Duration=3,Transition=undefined)">
<meta http-equiv="Content-Language" content="en-us">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<!-- <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">-->
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">

    <!--[if lt IE 9]>
    <script src="//html5shiv.googlecode.com/svn/trunk/html5.js"></script><script>provinfo.lt_ie9 = 1;</script>
	<![endif]-->
    <!--[if lte IE 9]>
    <script>provinfo.lte_ie9 = 1;</script>
    <![endif]-->


    <!--[if lt IE 9]>
    <script type="text/javascript" language="javascript" src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
    <script src="js/html5.js"></script>
	<![endif]-->
	<!--[if gte IE 9]>
    <script src="http://code.jquery.com/jquery-2.0.0b2.js"></script>
    <script src="js/html5.js"></script>
	<![endif]-->
	
<!-- Other Meta Tags -->
<meta http-equiv="Content-Language" content="en-us"/>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo $title; ?></title>
  
<!-- Translator ------>
<!-- Favicons -->
<link rel="shortcut icon" href="<?php echo BASE; ?>favicon.ico" />
<script type="text/javascript" src="<?php echo BASE; ?>assets/js/jquery.js"></script>

<!-- Other CSS -->
<link rel='stylesheet' type='text/css' href='<?php echo BASE; ?>assets/css/validationEngine.jquery.css'/>
<link href="<?php echo BASE; ?>assets/css/font-awesome.min.css" rel="stylesheet" type="text/css" />
<link href="<?php echo BASE; ?>assets/css/froala_editor.min.css" rel="stylesheet" type="text/css" />
<link rel="stylesheet" href="<?php echo BASE; ?>assets/css/paginate.css" type="text/css" />
<link rel="stylesheet" href="<?php echo BASE; ?>assets/css/select2.css" type="text/css" />
<link rel="stylesheet" type="text/css" href="<?php echo BASE; ?>assets/css/jquery.fancybox.css?v=2.1.5" media="screen" />
<link href='<?php echo BASE; ?>assets/css/style.css?21' rel='stylesheet' type='text/css'>

<!-- Load Dynamic Theme --->
<?php
$activeTheme = getSetting('activeTheme');
if(!empty($activeTheme)) { //Override default theme
	echo '<link href="'.BASE.'themes/'.$activeTheme.'/style.css?21" rel="stylesheet" type="text/css">';
}

?>

<link rel='stylesheet' type='text/css' href='<?php echo BASE; ?>assets/css/validationEngine.jquery.css'/>
<link href="<?php echo BASE; ?>assets/css/font-awesome.css" rel="stylesheet" type="text/css" />
<link href="<?php echo BASE; ?>assets/css/froala_editor.css" rel="stylesheet" type="text/css" /> 
<!-- set user defined background if available  --> 
<style>
	<?php
		$backgroundImage = getSetting('siteBackground');
		if($reseller > 0) {
		$backgroundImage = getResellerSetting('background_url',$reseller);	
		}
		$backgroundColor = getSetting('backgroundColor');
	?>
	body {
	
		background-image: url(<?php echo BASE; ?>media/uploads/<?php echo $backgroundImage; ?>);	
		background-color: <?php echo $backgroundColor; ?>;
	}
	
	<?php
		$headerColor = getSetting('headerColor');
		$fontColor = getSetting('fontColor');
		$headerFontColor = getSetting('headerFontColor');
		$menuColor = getSetting('menuColor');
	?>
	#header {
		background-color: <?php echo $headerColor; ?>;	
		color: <?php echo $headerFontColor; ?>;	
		border-bottom: 2px solid <?php echo $headerColor; ?>;
	}
	#logo {
		color: <?php echo $fontColor; ?>;
	} 
	#userProfile {
		color: <?php echo $headerFontColor; ?>;
	}
	#responsive-menu {
		color: <?php echo $headerFontColor; ?>;
		border: 1px solid <?php echo $headerFontColor; ?>;
	}
	#side-menu {
		background-color: <?php echo $menuColor; ?>;
		color: <?php echo $fontColor; ?>;
	}
	#side-menu a {
		color: <?php echo $fontColor; ?>;
	}

</style>


</head>

<body>


<?php include('drivers/includes/customAlertBox.php'); ?>

<?php if((!adminLogedIn() || !isset($_SESSION['manageUser'])) || isset($_GET['logout'])):require('includes/auth.inc'); endif ;
$UserID = $userID = getUser();

?>
<style>@import url(//fonts.googleapis.com/earlyaccess/droidarabickufi.css); </style>
<div id="header">
	<div id="logo"><img src="<?php echo BASE.$siteLogo; ?>" alt="<?php echo $siteName; ?>" /></div>
    <div id="responsive-menu" onclick="jQuery('#side-menu').toggle('show');"><i class="fa fa-navicon"></i></div> 
<div id="userProfile" class="desktop">السلام عليكم <name><?php echo getName(getUser()); ?></name> <img src="<?php echo BASE; ?>media/images/no-body.png" /></div>
</div>

<div id="body">
    <?php include(dirname( __FILE__ ).'/usermenu.php'); ?>
    <div class="desktop" style="width: 20%; min-width: 260px; float: left; height: 10px;"></div>
    <div id="main-body">
    	<div id="title">
        	<div class="title"><?php echo $title; ?></div>
            <div class="action desktop"><strong>اخر تسجيل دخول</strong>: <?php echo date('M d, Y', strtotime(userData('lastLogin',$userID))); ?>. &nbsp;&nbsp;&nbsp;<strong>IP</strong>: <?php echo userData('lastIP',$userID); ?></div>
        </div>

<?php if(!empty($message)) { showMessage($message, $class); } 

}//close if csv
?>