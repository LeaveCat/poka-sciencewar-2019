<?php
require_once "config.php";
if(isset($_COOKIE['token'])){
	$jwt = jwt_decode($_COOKIE['token']);
	if(!isset($jwt->uid)){
		die("Invalid Token");
	}
}else{
	$data = array(
		"uid" => rand(0,9999999),
		"money" => 100,
		"buy" => array(),
	);
	setcookie('token', jwt_encode($data));
	header('Location: ./');
}
?>
<b>FLAG Shop 2019</b><br/><br/>

Money : <?=$jwt->money;?><br/>

<table border=1>
	<tr>
		<td>Product</td>
		<td>money</td>
		<td>-</td>
	</tr>
<?php
foreach (array_keys($product) as $index) {
?>
	<tr>
		<td><?=$product[$index]["name"]?></td>
		<td><?=$product[$index]["money"]?></td>
		<td><a href="buy.php?id=<?=$index?>">Buy</a></td>
	</tr>
<?php
}
?>
</table>
<br/>

<?php
if($jwt->buy){
?>
------- Buy List -------<br/>
<table border=1>
	<tr>
		<td>Product</td>
		<td>money</td>
		<td>Comment</td>
	</tr>
<?php
	foreach(array_keys($jwt->buy) as $index) {
?>
	<tr>
		<td><?=$jwt->buy[$index]->name;?></td>
		<td><?=$jwt->buy[$index]->money;?></td>
		<td><?=htmlentities($jwt->buy[$index]->comment);?></td>
	</tr>
<?php
}
?>
</table>
<br/>
<a href='download.php'>Receipt Download</a>
<?php } ?>
