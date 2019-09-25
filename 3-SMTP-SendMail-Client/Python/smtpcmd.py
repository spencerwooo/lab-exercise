import cmd
import smtplib
from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.utils import parseaddr, formataddr


class MailCmd(cmd.Cmd):

    intro = 'Simple mail sender'
    prompt = '(mail) '
    mail_addr = ''
    username = ''
    password = ''
    server = None

    def do_set(self, line:str):
        'set server [server addr], set address [address], set username [username], set [password]'
        if len(line.split()) != 2:
            print('invalid command')
            return
        key, content = line.split()
        if key not in ('address', 'username', 'password', 'server'):
            print('invalid command')
            return
        if key == 'server':
            self.server = smtplib.SMTP(content, 25)
            self.server.set_debuglevel(1)
        elif key == 'address':
            if not self.server:
                print('plz set the server first')
                return
            self.mail_addr = content
        elif key == 'username':
            self.username = content
        else:
            if not self.server:
                print('plz set the server first')
                return
            if not self.mail_addr:
                print('plz set address first')
            else:
                self.password = content
                self.server.login(self.mail_addr, self.password)

    def do_EOF(self, line):
        return True

    def do_sendmail(self, line):
        target_addr = input('The address to send mail: ')
        target_username = input('Who you want to send: ')
        title = input("The mail title: ")
        print('Type the main body below, end with Ctrl-Z (Ctrl-D in Unix)')
        body = ''
        while True:
            try:
                body += input() + '\r\n'
            except EOFError:
                break
        msg = MIMEText(body, 'plain', 'utf-8')
        msg['From'] = formataddr((Header(self.username, 'utf-8').encode(), self.mail_addr))
        msg['To'] = formataddr((Header(target_username, 'utf-8').encode(), target_addr))
        msg['Subject'] = Header(title, 'utf-8').encode()
        self.server.sendmail(self.mail_addr, [target_addr], msg.as_string())

    def postloop(self):
        if self.server:
            self.server.close()

if __name__ == '__main__':
    MailCmd().cmdloop()