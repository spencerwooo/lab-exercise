import poplib
import email
import getpass
from email.parser import Parser
from email.header import decode_header
from email.utils import parseaddr

def decode_str(s):
    value, charset = decode_header(s)[0]
    if charset:
        value = value.decode(charset)
    return value

# indent用于缩进显示:
def print_info(msg, indent=0):
    if indent == 0:
        # 邮件的From, To, Subject存在于根对象上:
        for header in ['From', 'To', 'Subject']:
            value = msg.get(header, '')
            if value:
                if header=='Subject':
                    # 需要解码Subject字符串:
                    value = decode_str(value)
                else:
                    # 需要解码Email地址:
                    hdr, addr = parseaddr(value)
                    name = decode_str(hdr)
                    value = u'%s <%s>' % (name, addr)
            print('%s%s: %s' % ('  ' * indent, header, value))
    if (msg.is_multipart()):
        # 如果邮件对象是一个MIMEMultipart,
        # get_payload()返回list，包含所有的子对象:
        parts = msg.get_payload()
        for n, part in enumerate(parts):
            print('%spart %s' % ('  ' * indent, n))
            print('%s--------------------' % ('  ' * indent))
            # 递归打印每一个子对象:
            print_info(part, indent + 1)
    else:
        # 邮件对象不是一个MIMEMultipart,
        # 就根据content_type判断:
        content_type = msg.get_content_type()
        if content_type=='text/plain' or content_type=='text/html':
            # 纯文本或HTML内容:
            content = msg.get_payload(decode=True).decode()
            print('%sText: %s' % ('  ' * indent, content + '...'))
        else:
            # 不是文本,作为附件处理:
            print('%sAttachment: %s' % ('  ' * indent, content_type))

def main():
    host = input('plz input your server: ')
    username = input('plz input your email: ')
    password = getpass.getpass('and input your password: ')
    server =  poplib.POP3(host,1100)
    server.set_debuglevel(1)
    server.user(username)
    server.pass_(password)
    _, mail_list, _ = server.list()
    mail_len = len(mail_list)
    mail_content = []
    for i in range(mail_len):
        _, raw_content, _ = server.retr(i+1)
        try:
            msg_content = b'\r\n'.join(raw_content).decode()
            msg = Parser().parsestr(msg_content)
            mail_content.append(msg)
        except:
            mail_content.append(None)
    server.quit()
    title_list = []
    for i in mail_content:
        if i:
            title_list.append(decode_str(i['Subject']))
        else:
            title_list.append(None)
    for i, title in enumerate(title_list):
        if title:
            print('{}: {}'.format(i+1, title))
    while True:
        print("input the number of the mails u want to read, 0 to exit")
        try:
            no = int(input('number: '))
            if no == 0: break
            msg = mail_content[no-1]
        except:
            print("input error")
            continue
        print_info(msg)

if __name__ == '__main__':
    main()