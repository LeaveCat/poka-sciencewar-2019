<?php
	include("db.php");

	$user = $_GET['user'];
	$pass = $_GET['pass'];

	/*
	MariaDB [sqli]> desc userstbl;
	+-------+-------------+------+-----+---------+-------+
	| Field | Type        | Null | Key | Default | Extra |
	+-------+-------------+------+-----+---------+-------+
	| user  | binary(255) | NO   |     | NULL    |       |
	| pass  | binary(255) | NO   |     | NULL    |       |
	+-------+-------------+------+-----+---------+-------+
	2 rows in set (0.001 sec)

	MariaDB [sqli]> select * from userstbl;
	Empty set (0.000 sec)

	MariaDB [sqli]> 
	*/

	$filter = "/'|\"|\\\\|0x|0b|\(|\)|processlist/im";
	if (preg_match_all($filter, $user.$pass, $match) > 1) {
		$user = preg_replace($filter, "hi!", $user);
		$pass = preg_replace($filter, "hi!", $pass);
	}
	
	$result = mysqli_query($conn, "select * from userstbl where user='${user}' and pass='${pass}';");
	$row = mysqli_fetch_array($result);

	if ($row["user"]) {
		if ($row["user"] === "Sup3rAdm1n")
			die($flag);
 
		echo "Welcome ".$row["user"];
	}
   
	echo "<hr>";
	highlight_file(__FILE__);
?>
