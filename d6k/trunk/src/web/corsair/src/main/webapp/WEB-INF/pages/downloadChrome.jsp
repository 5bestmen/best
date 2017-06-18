<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/12/28
  Time: 10:13
  To change this template use File | Settings | File Templates.
--%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
  <title>chrome download</title>
  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
  <script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
  <script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
  <script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
  <script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
  <script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>

  <script type="text/javascript" src="<c:url value="/resources/extjs/ext-all.js"/>"></script>
  <script type="text/javascript" src="<c:url value="/resources/js/common.js"/>"></script>
  <script type="text/javascript">

  </script>
</head>
<body>
<div style="width:100%;height:100%;text-align:center;margin:0px auto;" >
  <form:form action="downloadChrome" method="post" commandName="downloadChromeForm" >
    <form:input type="image" src="/resources/img/chrome.png"  usemap="#chromemap"   alt="chrome"  path="downloadChrome" cssStyle="margin-top: 18%;" onclick="submit()" />
    <map name="chromemap" id="chromemap">
      <area  shape="rect"   coords="0,0,210,260"  href =""/>
    </map>
    <h4>为了更好的用户体验，请使用chrome浏览器访问光伏系统</h4>
    <h4>点击图标下载chrome浏览器</h4>
  </form:form>

</div>

</body>
</html>

