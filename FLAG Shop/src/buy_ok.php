<?php
require_once "config.php";
if(isset($_COOKIE['token'])){
	$jwt = jwt_decode($_COOKIE['token']);
	if(!isset($jwt->uid)){
		die("Invalid Token");
	}
}else{
	header('Location: ./');
}

if(!isset($_GET['id']) || strlen($_GET['id']) === 0 ){
	die("Invalid id");
}

$idx = (int)$_GET['id'];

if($product[$idx]){
	if( (int)$product[$idx]["money"] > (int)$jwt->money ) die("insufficient money");
	if( $product[$idx]["name"] === "FLAG" ) { system("./flag_9fc984edbb43766f0956b7b38ad4d6b7"); die(); }


	$data = $jwt;
	$data->money -= (int)$product[$idx]["money"];
	array_push($data->buy, array(
		"name" => $product[$idx]["name"],
		"money" => (int)$product[$idx]["money"],
		"comment" => $_POST['comment'],
	));
	
	setcookie('token', jwt_encode($data));
	header('Location: ./');

}
die("Invalid id");
?>