<?php

require_once 'jwt/BeforeValidException.php';
require_once 'jwt/ExpiredException.php';
require_once 'jwt/SignatureInvalidException.php';
require_once 'jwt/JWT.php';
use \Firebase\JWT\JWT;

function jwt_encode($data){
	global $Key;
	return JWT::encode($data, $Key, 'HS256');
}

function jwt_decode($token){
	global $Key;
	return JWT::decode($token, $Key, array('HS256'));
}



$product = array(
	0 => array(
		"name" => "Test",
		"money" => 100,
		),
	1 => array(
		"name" => "FLAG",
		"money" => 10000000000000,
	),
);


$if_you_want_to_get_key = "go Down !! you can't just see it.";


?>
<!--

<?php

$Key = "Th1s_1s_n0t_FLAG_!!!find_flag!!!____gogogogogo____";

?>

-->