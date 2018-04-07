# C-Socket-Send-a-message
一个库，可以用来向网站发送报文。
用法：
HTTPRES:进行HTTP操作的类
.dectype():选择请求方式
.dechost():选择主机
.decver():选择http版本
.addhead():添加请求头
.addurl():添加/后url（GET）
.hsend():发送报文
.hsend()返回结构体RESP
原型：
struct RESP {
	int ErrorCod;//返回的错误代码
	int Http1, Http2;//两个版本号
	int RESPcod;//返回的状态码
	string Sresp;//完整报文
};

