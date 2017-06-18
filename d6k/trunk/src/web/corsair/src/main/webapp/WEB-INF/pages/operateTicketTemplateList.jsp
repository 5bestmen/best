<%--
  Created by IntelliJ IDEA.
  User: zjq
  Date: 2015/10/19
  Time: 15:53
  To change this template use File | Settings | File Templates.
--%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>

<html lang="zh_CN">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">

  <title>生成操作票</title>



  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/font-awesome.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

  <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
  <script src="dist/js/html5shiv.min.js"></script>
  <script src="dist/js/respond.min.js"></script>
  <![endif]-->

  <%--todo 页面自身需要的CSS begin--%>
  <%--页面自身需要的CSS end--%>
</head>
<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.min.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/dist/js/bootbox.min.js"/>"></script>
<script type="application/javascript">
  $(function(){
    $("tbody tr:even").css("background-color","rgb(31, 86, 134)");
    $("tbody tr:even").children().css("color","white");
    $("tbody tr:odd").css("background","#82accb");
  });

</script>
<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>
  <div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-12">
        <h1 class="page-header">生成操作票</h1>
      </div>
      <!-- /.col-lg-12 -->
    </div>
    <!-- /.row -->

    <form:form cssClass="form-inline" action="operateTicketTemplateList" method="post" commandName="operateTicketTemplateListForm">
      <form:hidden path="deleteIds"/>
      <form:input type="submit" value="删除" cssStyle="display:none" path="del"></form:input>
      <a class="btn btn-primary" href="operateTicketTemplateEdit">
        <i class="glyphicon glyphicon-plus"></i>
        新增模板
      </a>
      <a class="btn btn-primary" href="operateTicketEdit">
        <i class="glyphicon glyphicon-wrench"></i>
        自定义操作票
      </a>
      <div style="margin:10px 0;"></div>
      <div class="row">
        <div class="col-lg-12 col-md-12">
          <div class="table-responsive">
            <table class="table table-bordered table-striped">
              <thead>
              <tr id="first">
                <th>
                  操作票模板名称
                </th>
                <th>
                  生成操作票
                </th>
                <th>
                  编辑
                </th>
                <th>
                  删除
                </th>
                <th>
                  操作记录
                </th>
              </tr>
              </thead>
              <tbody>
              <c:if test="${not empty commonTicketList}">
                <c:forEach items="${commonTicketList}" var="commonTicket">
                  <tr>
                    <td>
                      <c:out value="${commonTicket.name}"/>
                    </td>

                    <td align="center">
                      <a href="operateTicketEdit?operateTicketTemplateId=${commonTicket.id }">
                        <i class="glyphicon glyphicon-plus"></i>
                        生成操作票</a>
                    </td>

                    <td align="center">
                      <a  href="operateTicketTemplateEdit?id=${commonTicket.id }">
                        <i class="glyphicon glyphicon-edit"></i>
                        编辑</a>
                    </td>

                    <td align="center">
                      <a
                         href="javascript:if(confirm('是否确定删除这条记录？')){document.forms[0].deleteIds.value='${commonTicket.id}';document.forms[0].del.click();}">
                        <i class="glyphicon glyphicon-remove"></i>
                        删除
                      </a>
                    </td>

                    <td align="center">
                      <a href="operateTicketProcessRecord?id=${commonTicket.id }&&type=template">
                        <i class="glyphicon glyphicon-arrow-down"></i>
                        操作记录</a>
                    </td>
                  </tr>
                </c:forEach>
              </c:if>
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </form:form>

 <%-- </div>
</div>--%>
</body>
</html>

