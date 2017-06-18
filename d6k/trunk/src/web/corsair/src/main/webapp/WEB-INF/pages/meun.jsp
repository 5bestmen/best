<%--
  Created by IntelliJ IDEA.
  User: 洪祥
  Date: 2015/6/15
  Time: 11:06
  To change this template use File | Settings | File Templates.
--%>
<%@ page import="org.springframework.web.context.support.WebApplicationContextUtils" %>
<%@ page import="com.corsair.service.ApplicationInformationService" %>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<%
    ApplicationInformationService service = (ApplicationInformationService) WebApplicationContextUtils
            .getRequiredWebApplicationContext(session.getServletContext()).getBean("applicationInformationService");

    String appName = service.getAppName();
%>
<!DOCTYPE html>
<link href="<c:url value="/resources/css/commonStyle.css"/>" rel="stylesheet" type="text/css">
<%--script type="application/javascript">

    var RunTime = {};

    RunTime.socket = null;

    RunTime.connect = (function (host) {
        if ('WebSocket' in window) {
            RunTime.socket = new WebSocket(host);
        } else if ('MozWebSocket' in window) {
            RunTime.socket = new MozWebSocket(host);
        } else {
            //Console.log('Error: WebSocket is not supported by this browser.');
            return;
        }

        RunTime.socket.onopen = function () {
            //Console.log('Info: WebSocket connection opened.');

        };

        RunTime.socket.onclose = function () {
            //Console.log('Info: WebSocket closed.');
            Chat.initialize();
        };

        RunTime.socket.onmessage = function (message) {
            document.getElementById("sysRunTime").innerHTML = message.data;
        };
    });

    RunTime.initialize = function () {
        if (window.location.protocol == 'http:') {
            RunTime.connect('ws://'+ window.location.host +'/runtime');
        } else {
            RunTime.connect('wss://'+ window.location.host +'/runtime');
        }
    };


    RunTime.initialize();
</script--%>


<%--<script>--%>
<%--window.onload=function(){--%>
<%--var runTime =  "<c:out value="${sessionScope.RUNNING_TIME}"/>";--%>
<%--document.getElementById("sysRunTime").innerHTML = runTime;--%>
<%--}--%>
<%--</script>--%>
<!-- 导航，包含上部和左侧 -->
<%--<nav class="navbar navbar-default navbar-static-top" role="navigation" style="margin-bottom: 0">--%>
<%--左上角主菜单链接--%>
<%--<div class="navbar-header">--%>
<%--<button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">--%>
<%--<span class="sr-only">Toggle navigation</span>--%>
<%--<span class="icon-bar"></span>--%>
<%--<span class="icon-bar"></span>--%>
<%--<span class="icon-bar"></span>--%>
<%--</button>--%>
<%--<a class="navbar-brand" href="index.do">${sessionScope.APP_NAME}</a>--%>
<%--</div>--%>
<%--<!-- /.navbar-header -->--%>

<%--&lt;%&ndash;上部导航条&ndash;%&gt;--%>
<%--<div id="wrapper">
    <div id="page-wrapper">--%>
<%--<div class="row" style="background: url(/resources/img/top_bg.png) no-repeat;background-size:cover ;">
    <div class="panel-heading" style="padding: 10px 25px;">
        <div style="float: left"><span class="panel-title" style="font-size: 24px;"><%=appName%></span>
            <span>SIGN TO SYSTEM</span>
        </div>
        &lt;%&ndash;<div style="float: right;margin-top:10px">
            <div class="btn-group">
                <button class="btn btn-mini" id="changeMenu" style="color:#000;">切换</button>
            </div>
        </div>&ndash;%&gt;
        <div style="float: right">
            <ul class="nav navbar-top-links navbar-right">

                &lt;%&ndash;上部导航条的用户部分&ndash;%&gt;
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-user fa-fw"></i>${com_user.account.realname}<i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-user">
                        &lt;%&ndash;<li><a href="#"><i class="fa fa-user fa-fw"></i>${com_user.account.realname}</a>&ndash;%&gt;
                        <li><a href="logout"><i class="fa fa-sign-out fa-fw"></i>注销</a>
                        <li><a href="pwdModify"><i class="fa fa-wrench fa-fw"></i>修改口令</a>
                        </li>
                    </ul>
                    <!-- /.dropdown-user -->
                </li>
                <!-- /.dropdown -->
            </ul>
        </div>
    </div>
</div>--%>
<%--横向导航条,0是侧边菜单，不是0 的都是横向菜单--%>
<c:choose>
    <c:when test="${sessionScope.MENU_WAY !='0'}">
        <div  id="navbar-main" >
            <ul class="nav navbar-nav">
                <li class="dropdown">
                        <c:set var="index" value="0" scope="request"/>
                    <c:forEach var="currentItem" items="${com_user.menuList}" varStatus="vs">
                    <c:choose>
                    <c:when test="${fn:length(currentItem.children) == 0}">
                <li class="title">
                    <a href="${currentItem.url}?menuId=1" target="main">${currentItem.text}</a>
                </li>
                </c:when>
                <c:otherwise>
                    <%--有子菜单时，需要一个li套ul--%>
                    <li>
                        <a class="dropdown-toggle" data-toggle="dropdown" href="#" id="download">${currentItem.text}<span
                                class="caret"></span></a>
                        <ul class="dropdown-menu" aria-labelledby="download">
                            <c:forEach var="currentMenu" items="${currentItem.children}" varStatus="vs">
                                <c:choose>
                                    <%--二级菜单下没有三级菜单--%>
                                    <c:when test="${fn:length(currentMenu.children) == 0}">
                                        <c:choose>
                                            <c:when test="${fn:contains(currentMenu.url, 'pvarray')}">
                                                <li class="special"><a href="${currentMenu.url}&menuId=1" target="main">${currentMenu.text}</a></li>
                                            </c:when>
                                            <c:otherwise>
                                                <li class="special"><a href="${currentMenu.url}?menuId=1" target="main">${currentMenu.text}</a></li>
                                            </c:otherwise>
                                        </c:choose>
                                    </c:when>
                                    <%--二级菜单下有三级菜单--%>
                                    <c:otherwise>
                                        <li>
                                            <a href="${currentMenu.url}">${currentMenu.text}<span
                                                    class="fa arrow" target="main"></span></a>
                                            <ul class="nav nav-third-level">
                                                <c:forEach var="thirdMenu" items="${currentMenu.children}" varStatus="vs">
                                                    <li><a href="${thirdMenu.url}?menuId=1" target="main">${thirdMenu.text}</a></li>
                                                </c:forEach>
                                            </ul>
                                        </li>
                                    </c:otherwise>
                                </c:choose>
                            </c:forEach>
                        </ul>
                    </li>
                </c:otherwise>
                </c:choose>
                </c:forEach>
            </ul>
        </div>
        <br/>
        <br/>
        <script>
            $(function(){
                $("#page-wrapper").css("margin","0px");
            });
        </script>
    </c:when>
    <c:otherwise>
        <%--左侧导航窗口--%>
        <%--<div class="navbar-default sidebar m-t-xs" role="navigation" >--%>
        <div class="navbar-default sidebar m-t-xs">
            <div class="sidebar-nav navbar-collapse">
                <ul class="nav" id="side-menu">
                    <li class="sidebar-search">
                        <div class="dropdown profile-element">
                            <div class="row" style="margin-right: -15px;border-bottom: 2px white solid;position: relative;z-index: 2;">
                                    <%--<div class="col-lg-12">--%>
                                    <%--<img alt="image" class="img-responsive aligncenter" src="<c:url value="/resources/img/sun.png"/>"/>--%>
                                    <%--</div>--%>
                                <div col-lg-12>
                                        <%--<ul class="pull-center" style="list-style:none">--%>
                                        <%--<li>电站名称：${sessionScope.STATION_NAME}</li>--%>
                                        <%--<li>电站装机容量：${sessionScope.STATION_CAPACITY}</li>--%>
                                        <%--<li>电站并网容量：${sessionScope.POWER_GRID_CAPACITY}</li>--%>
                                        <%--<li>电站在网运行时间：${sessionScope.RUNNING_TIME}</li>--%>
                                        <%--</ul>--%>
                                        <%--<table class="table table-responsive">
                                            <tbody>
                                            <tr>
                                                <td>
                                                    电站名称
                                                </td>
                                                <td>
                                                    ${sessionScope.STATION_NAME}
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    电站装机容量
                                                </td>
                                                <td>
                                                    ${sessionScope.STATION_CAPACITY}
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    电站并网容量
                                                </td>
                                                <td>
                                                    ${sessionScope.POWER_GRID_CAPACITY}
                                                </td>
                                            </tr>
                                            <tr>
                                                <td>
                                                    并网运行时间
                                                </td>
                                                <td id="sysRunTime">
                                                    &lt;%&ndash;${sessionScope.RUNNING_TIME}&ndash;%&gt;
                                                </td>
                                            </tr>

                                            </tbody>
                                        </table>--%>
                                    <div class="col-lg-12" style=" border-bottom: 2px white solid;margin-bottom: 12px;">
                                        <div class="menu-heading" style="float:left">电站名称:</div>
                                        <div class="menu-heading"
                                             style="float:left;color:#f18e14">${sessionScope.STATION_NAME}</div>
                                    </div>
                                    <div class="col-lg-12" style=" margin-bottom: 12px;">
                                        <div class="menu-heading">电站装机容量</div>
                                        <div class="menu-heading">
                                            <div class="menu-phalanx">${sessionScope.STATION_CAPACITY}</div>
                                        </div>
                                        <div class="menu-heading">电站并网容量</div>
                                        <div class="menu-heading">
                                            <div class="menu-phalanx">${sessionScope.POWER_GRID_CAPACITY}</div>
                                        </div>
                                        <%--<div class="menu-heading">并网运行时间</div>--%>
                                        <%--<div class="menu-heading">--%>
                                            <%--<div class="menu-phalanx" id="sysRunTime">${sessionScope.RUNNING_TIME}</div>--%>
                                        <%--</div>--%>

                                    </div>
                                </div>

                                    <%--<div class="col-lg-12 text-center">--%>
                                    <%--<a data-toggle="dropdown" class="dropdown-toggle" href="index.do">--%>
                                    <%--<span class="clear">--%>
                                    <%--<span class="block m-t-xs">--%>
                                    <%--<strong class="font-bold">${com_user.account.realname}</strong>--%>
                                    <%--</span>--%>
                                    <%--</span>--%>
                                    <%--</a>--%>
                                    <%--<ul class="dropdown-menu animated fadeInRight m-t-xs">--%>
                                    <%--<li><a href="pwdModify">修改口令</a>--%>
                                    <%--<li><a href="logout">注销</a></li>--%>
                                    <%--</ul>--%>
                                    <%--</div>--%>
                                <div style="position: absolute;z-index: 3;margin-left: 80px;margin-top: 188px;">
                                    <div class="box-sign"></div>
                                    <div class="box-sign"></div>
                                    <div class="box-sign"></div>
                                </div>
                            </div>
                        </div>
                        <div class="menu-sign"></div>
                        <%--<div class="menu-sign">Navigation</div>--%>
                    </li>

                    <c:set var="index" value="0" scope="request"/>
                        <%--<c:set var="level" value="2" scope="request" />--%>
                        <%--注意这里的侧边菜单栏名称不能定义为menuList，会和菜单管理页面的menuList名称冲突--%>
                        <%--<c:set var="sideBarList" value="${com_user.menuList}" scope="request" />--%>
                    <c:forEach var="currentItem" items="${com_user.menuList}" varStatus="vs">
                        <%--没有子菜单的情况下只需要一个li--%>
                        <c:choose>
                            <c:when test="${fn:length(currentItem.children) == 0}">
                                <li class="title">
                                    <a href="${currentItem.url}" target="main" ><%--<i
                                        class="fa ${currentItem.icon} fa-fw"></i>--%>${currentItem.text}</a>
                                </li>
                            </c:when>
                            <c:otherwise>
                                <%--有子菜单时，需要一个li套ul--%>
                                <li>
                                    <a href="#"><%--<i class="fa ${currentItem.icon} fa-fw"></i>--%>${currentItem.text}<span
                                            class="fa arrow"></span></a>
                                    <ul class="nav nav-second-level">
                                        <c:forEach var="currentMenu" items="${currentItem.children}" varStatus="vs">
                                            <c:choose>
                                                <%--二级菜单下没有三级菜单--%>
                                                <c:when test="${fn:length(currentMenu.children) == 0}">
                                                    <li class="special"><a href="${currentMenu.url}" target="main"><%--<i
                                                        class="fa ${currentItem.icon} fa-fw"></i>--%>${currentMenu.text}</a>
                                                    </li>
                                                </c:when>
                                                <%--二级菜单下有三级菜单--%>
                                                <c:otherwise>
                                                    <li class="title">
                                                        <a href="${currentMenu.url}" ><%--<i class="fa ${currentItem.icon} fa-fw"></i>--%>${currentMenu.text}<span
                                                                class="fa arrow"></span></a>
                                                        <ul class="nav nav-third-level">
                                                            <c:forEach var="thirdMenu" items="${currentMenu.children}"
                                                                       varStatus="vs">
                                                                <li class="special"><a href="${thirdMenu.url}" target="main"><%--<i
                                                                class="fa ${currentItem.icon} fa-fw"></i>--%>${thirdMenu.text}</a>
                                                                </li>
                                                            </c:forEach>
                                                        </ul>
                                                    </li>
                                                </c:otherwise>
                                            </c:choose>
                                        </c:forEach>
                                    </ul>
                                </li>
                            </c:otherwise>
                        </c:choose>
                    </c:forEach>
                </ul>
            </div>
            <!-- /.sidebar-collapse -->
        </div>


        <script>
            $(function(){
                $("#page-wrapper").css("margin","0px 0px 0px 250px");
            });
        </script>
    </c:otherwise>
</c:choose>







