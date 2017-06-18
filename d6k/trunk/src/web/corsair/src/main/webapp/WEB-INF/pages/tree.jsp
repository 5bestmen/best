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
<link href="<c:url value="/resources/tree/easyui.css"/>" rel="stylesheet" type="text/css">
<link href="<c:url value="/resources/tree/icon.css"/>" rel="stylesheet" type="text/css">
<link href="<c:url value="/resources/tree/demo.css"/>" rel="stylesheet" type="text/css">

<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/tree/jquery.easyui.min.js"/>"></script>
<script>
    $(function() {
        $("#tree").css("height",document.documentElement.clientHeight - 200)
        $("#tree").css("width","200px")
    });
</script>

<%--左侧导航窗口--%>







