import ftplib
import sys

destination = "pi-v4.felinae98.ml"
ftp = ftplib.FTP()
ftp.connect(destination,2121)
print(ftp.getwelcome())
ftp.login('spencer','123')

def main():
	print("YOU CAN u/d/l/q")
	welcome_word = input("WHAT WOULD YOU WANT?")
	if welcome_word == "u":
		file_path = input("SHOW ME THE PATH:")
		upload = open(file_path,'rb')
		name = input("WHAT IS YOUR NEW NAME:")
		ftp.storbinary("STOR %s"% name,upload)
		upload.close()
		print("SUCCESS!")
	

	elif welcome_word == "d":
		file_name = input("SHOW ME THE NAME:")
		dest_path = "/home/brandon"	+ file_name
		download = open(dest_path,'wb')
		ftp.retrbinary("RETR %s" % file_name, download.write)
		fd.close()
		print("SUCCESS!")

	elif welcome_word == "l":
		ftp.dir()

	else:
		print("Bye ya~")
	ftp.quit()


if __name__ == '__main__':
    main()