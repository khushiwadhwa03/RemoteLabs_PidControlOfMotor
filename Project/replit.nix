{ pkgs }: {
	deps = [
		pkgs.sudo
  pkgs.nodejs-16_x
  pkgs.mariadb
  pkgs.python39Packages.pip
  pkgs.php74
    pkgs.php74Extensions.pdo
    pkgs.sqlite
	];
}