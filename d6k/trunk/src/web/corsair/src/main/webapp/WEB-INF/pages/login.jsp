<%@ page import="org.springframework.web.context.support.WebApplicationContextUtils" %>
<%@ page import="com.corsair.service.ApplicationInformationService" %>
<%@ page import="com.rbac.entity.SysAccount" %>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<%
    ApplicationInformationService service = (ApplicationInformationService) WebApplicationContextUtils
            .getRequiredWebApplicationContext(session.getServletContext()).getBean("applicationInformationService");

    String appName = service.getAppName();
//    String appName = "sssss";
%>

<!DOCTYPE html>
<html lang="zh_CN">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title><%=appName%>-登录</title>
    <!-- Bootstrap Core CSS -->
    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">


    <!-- MetisMenu CSS -->
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">


    <!-- Custom CSS -->
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
    <script src="/resources/dist/js/html5shiv.min.js"></script>
    <script src="/resources/dist/js/respond.min.js"></script>
    <![endif]-->
    <style type="text/css">
        body {
            background: url(/resources/img/Bg.png) no-repeat;
            background-size: cover;
            font-family: "Microsoft YaHei";
        }

        .panel-default > .panel-heading {
            color: #337ab7;
            background-color: #fff;
            padding: 15px 15px 20px 15px;
        }

        .panel-title {
            font-size: 24px;
        }

        .panel-sign {
            color: #999;
            margin-top: 10px;
            font-size: 18px;
        }

        span {
            font-size: 16px;
            color: #666;
        }

        .form-group {
            margin-bottom: 10px;
        }

        .panel-body {
            padding: 30px 25px 5px 25px;;
        }

        .form-control {
            border: 1px solid #337ab7;
            border-radius: 0px;
        }

        .btn-lg {
            border-radius: 0px;
            margin-bottom: 15px;
        }

        .btn-success {
            background-color: #337ab7;
            border-color: #337ab7;
        }

        .checkbox {
            margin-bottom: 20px
        }

        .btn-success:hover {
            background-color: #337ab7;
            border-color: #4cae4c
        }

        .btn-success .badge {
            color: #337ab7;
            background-color: #fff
        }

        input:-webkit-autofill {
            -webkit-box-shadow: 0 0 0px 1000px #f1f1f1 inset;
            border: 1px solid #538fc2 !important;
        }

        .show {
            position: absolute;
            clip: rect(0px 100px 50px 0px);

        }
    </style>
</head>

<body>
<script>
    function isBrowser() {
        var Sys = {};
        var ua = navigator.userAgent.toLowerCase();
        var s;
        //window.navigator.userAgent.indexOf("Chrome")
        (s = ua.match(/msie ([\d.]+)/)) ? Sys.ie = s[1] :
                (s = ua.match(/firefox\/([\d.]+)/)) ? Sys.firefox = s[1] :
                        (s = ua.match(/chrome\/([\d.]+)/)) ? Sys.chrome = s[1] :
                                (s = ua.match(/opera.([\d.]+)/)) ? Sys.opera = s[1] :
                                        (s = ua.match(/version\/([\d.]+).*safari/)) ? Sys.safari = s[1] : 0;
        if (Sys.chrome) {//Js判断为谷歌chrome浏览器
        }
        else {
            //window.location.href = "downloadChrome";
        }
    }
    window.onload = isBrowser;
</script>
<div class="container">
    <div class="row" style="margin-top: 50px;">
        <div class="col-md-4 col-md-offset-4">
            <div class="login-panel panel panel-default">
                <div class="panel-heading">
                    <div><h3 class="panel-title"><%=appName%>
                    </h3></div>
                    <div style="margin-top: 10px;float: right;padding-right: 23px;"><img class="show"
                                                                                         src="/resources/img/1.png">
                    </div>
                    <div class="panel-sign">SIGN TO SYSTEM</div>
                </div>

                <div class="panel-body">
                    <form role="form" action="login" method="post">
                        <c:if test="${error_msg ne null}">
                            <div class="alert alert-danger" role="alert">用户名密码错误！</div>
                        </c:if>
                        <fieldset>
                            <div class="form-group">
                                <span>用户名/账户</span>
                            </div>
                            <div class="form-group">
                                <input class="form-control" placeholder="用户名" name="username" autofocus>
                            </div>
                            <div class="form-group" style="margin-top:20px;">
                                <span>密码</span>
                            </div>
                            <div class="form-group">
                                <input class="form-control" placeholder="密码" name="password" type="password" value="">
                            </div>
                            <%--<div class="checkbox">
                                <label>
                                    <input name="remember" type="checkbox" value="Remember Me">保持我的登录状态
                                </label>
                            </div>--%>

                            <input type="submit" class="btn btn-lg btn-success btn-block" value="登录"/>

                            <div style="margin-left: 50px;"><img style="width: 15%" src="/resources/img/2.png"></div>
                        </fieldset>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
</body>
</html>
