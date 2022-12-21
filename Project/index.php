<!DOCTYPE html>
<?php
//starting the session
session_start();
?>
<html lang="en">

<head>
	<meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1" />
	<!-- Bootstrap -->
	<link rel="stylesheet" type="text/css" href="css/styles.css" />
</head>

<body>





	<div class="login-box">
		<h1>Register</h1>
		<form method="POST" action="save_member.php">
			<div class="user-box">
				<input type="text" name="username" required="required">
				<label>Username</label>
			</div>
			<div class="user-box">
				<input type="password" name="password" required="required">
				<label>Password</label>
			</div>
			<div class="user-box">
				<input type="text" name="firstname" required="required" />
				<label>Firstname</label>
			</div>
			<div class="user-box">
				<input type="text" name="lastname" required="required" />
				<label>Lastname</label>
			</div>


		

			<!-- <button name="login" class="custom-btn btn-9">
				<span></span>
				<span></span>
				<span></span>
				<span></span>
				Log in
			</button> -->
			<br>
			<button name="register" class="btn">
				<svg width="150px" height="40px" viewBox="0 0 150 40" class="border">
					<polyline points="149,1 149,39 1,39 1,1 149,1" class="bg-line" />
					<polyline points="149,1 149,39 1,39 1,1 149,1" class="hl-line" />
				</svg>
				<span>Register</span>
			</button>
			<br>
			<br>
			<br>
			<br>
			<br>
			<div class="signup">Already a Member?</div>
			<br>
			<a href="login.php">
				<span></span>
				<span></span>
				<span></span>
				<span></span>
				Login
			</a>
		</form>
	</div>






	<!-- Registration Form start -->
	<!-- <form method="POST" action="save_member.php">	
				<div class="alert alert-info">Registration</div>
				<div class="form-group">
					<label>Username</label>
					<input type="text" name="username" class="form-control" required="required"/>
				</div>
				<div class="form-group">
					<label>Password</label>
					<input type="password" name="password" class="form-control" required="required"/>
				</div>
				<div class="form-group">
					<label>Firstname</label>
					<input type="text" name="firstname" class="form-control" required="required"/>
				</div>
				<div class="form-group">
					<label>Lastname</label>
					<input type="text" name="lastname" class="form-control" required="required"/>
				</div>
				<button class="btn btn-primary btn-block" name="register"><span class="glyphicon glyphicon-save"></span> Register</button>
			</form>	
			Registration Form end
		</div>
	</div> -->
</body>

</html>