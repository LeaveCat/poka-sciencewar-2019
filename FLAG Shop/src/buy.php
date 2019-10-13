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
?>
<table border=1>
	<tr>
		<td>Product</td>
		<td>money</td>
		<td>Comment</td>
		<td>-</td>
	</tr>
	<tr>
		<form action="buy_ok.php?id=<?=$idx?>" method="post">
		<td><?=$product[$idx]["name"];?></td>
		<td><?=$product[$idx]["money"];?></td>
		<td><input type="text" name="comment"></td>
		<td><input type="submit" value="Buy !"></td>
		</form>
	</tr>
</table></br>
<a href="index.php">Home</a>
<?php
}else{
	die("Invalid id");
}
?>
