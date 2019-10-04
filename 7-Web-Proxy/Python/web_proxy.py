import urllib
import urllib.request

#代理开关
proxySwitch = True

proxy = {"http":"101.201.199.194:8080"}
headers  = {"User-Agent":"Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0"}
url = "http://httpbin.org/get"

#构建两个handler，一个使用代理IP 一个不使用代理IP
httpproxy_handler = urllib.request.ProxyHandler(proxy)
nullproxy_handler = urllib.request.ProxyHandler({})

if proxySwitch:
    opener = urllib.request.build_opener(httpproxy_handler)
else:
    opener = urllib.request.build_opener(nullproxy_handler)

request = urllib.request.Request(url,headers = headers)
response = opener.open(request)
print(response.getcode())
print (response.read().decode('utf-8'))