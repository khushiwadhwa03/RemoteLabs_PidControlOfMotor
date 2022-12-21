<!DOCTYPE html>
<?php
//starting the session
session_start();
?>
<html lang="en">

<head>
	<meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1" />
	<link rel="stylesheet" type="text/css" href="css/styles.css" />
</head>

<body>
	<!-- <nav class="navbar navbar-default">
		<div class="container-fluid">
			<a class="navbar-brand" href="https://sourcecodester.com">Sourcecodester</a>
		</div>
	</nav> -->
	<!-- <div class="col-md-3"></div>
	<div class="col-md-6 well">
		<h3 class="text-primary">PHP - Login And Registration To Sqlite Using PDO</h3>
		<hr style="border-top:1px dotted #ccc;" />
		Link for redirecting page to Registration page
		<a href="index.php">Not a member yet? Register here...</a>
		<br style="clear:both;" /><br />
		<div class="col-md-3"></div>
		<div class="col-md-6"> -->




	<div class="login-box login-box2">
		<h1>Login</h1>
		<form method="POST" action="login_query.php">
			<div class="user-box">
				<input type="text" name="username" required="required">
				<label>Username</label>
			</div>
			<div class="user-box">
				<input type="password" name="password" required="required">
				<label>Password</label>
			</div>
			<!-- <button name="login" class="custom-btn btn-9">
				<span></span>
				<span></span>
				<span></span>
				<span></span>
				Log in
			</button> -->
			<br>
			
			<button name="login" class="btn">
				<svg width="150px" height="40px" viewBox="0 0 150 40" class="border">
					<polyline points="149,1 149,39 1,39 1,1 149,1" class="bg-line" />
					<polyline points="149,1 149,39 1,39 1,1 149,1" class="hl-line" />
				</svg>
				<span>Login</span>
			</button>
			<br>
			<br>
			<br>
			<br>
			<br>
			<div class="signup">Not a Member Yet?</div>
			<br>
			<a href="index.php">
				<span></span>
				<span></span>
				<span></span>
				<span></span>
				Register
			</a>
		</form>
	</div>



	<!-- Login Form Starts -->
	<!-- <form method="POST" action="login_query.php">
				<div class="alert alert-info">Login</div>
				<div class="form-group">
					<label>Username</label>
					<input type="text" name="username" class="form-control" required="required" />
				</div>
				<div class="form-group">
					<label>Password</label>
					<input type="password" name="password" class="form-control" required="required" />
				</div>
				
				<button class="btn btn-primary btn-block" name="login"><span class="glyphicon glyphicon-log-in"></span>
					Login</button>
			</form>
			Login Form Ends
		</div>
	</div> -->
</body>

</html>