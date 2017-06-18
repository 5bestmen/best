<%--
  Created by IntelliJ IDEA.
  User: zjq
  Date: 2015/11/6
  Time: 14:36
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

  <title>物资信息表</title>



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
<script type="javascript">
  function showMore(){
    var current_tr = $(this).parent.parent.next();
    if (current_tr.is(':visible')){
      current_tr.show();
    }else{
      current_tr.hide();
    }
  }
</script>
<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>
  <div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-12">
        <h1 class="page-header">物资信息</h1>
      </div>
      <!-- /.col-lg-12 -->
    </div>
    <!-- /.row -->

    <form:form cssClass="form-inline" action="materialInfomation" method="post" commandName="materialInformationForm">
      <form:hidden path="currentNo"/>

      <div class="form-group">
        <form:input cssClass="form-control" placeholder="物料编号" path="materialSeriesNo"/>
      </div>
      <div class="form-group">
        <form:input cssClass="form-control" placeholder="物料固定id" path="materialId"/>
      </div>
      <form:input type="submit" cssClass="btn btn-primary" value="查询物料" path="search"/>

      <a class="btn btn-primary" href="materialStoreInEdit">
        <i class="glyphicon glyphicon-plus"></i>
        新增出库
      </a>
      <a class="btn btn-primary" href="materialStoreOutEdit">
        <i class="glyphicon glyphicon-plus"></i>
        新增入库
      </a>
      <div style="margin:10px 0;"></div>
      <div class="row">
        <div class="col-lg-12 col-md-12">
          <div class="table-responsive">
            <table class="table table-bordered table-striped">
              <thead>
              <tr id="first">
                <th>
                  仓库名
                </th>
                <th>
                  库存数量
                </th>
                <th>
                  库存上限
                </th>
                <th>
                  库存下限
                </th>
                <th>
                  出入库信息
                </th>
                <th>
                  入库
                </th>
                <th>
                  出库
                </th>
              </tr>
              </thead>
              </tbody>
              <c:if test="${not empty materialStores}">
                <c:forEach items="${materialStores}" var="materialStore">
                  <tr>
                    <td>
                      <c:out value="${materialStore.materialWarehouse}"/>
                    </td>
                    <td>
                      <c:out value="${materialStore.stockNumber}"/>
                    </td>
                    <td>
                      <c:out value="${materialStore.inventoryUpperlimit}"/>
                    </td>
                    <td>
                      <c:out value="${materialStore.inventoryfloor}"/>
                    </td>

                    <td align="center">
                      <a  href="materialInfomationDetail?materialSeriesNo=${materialStore.id}">
                        <i class="glyphicon glyphicon-info-sign"></i>
                        出入库信息</a>
                    </td>
                    <td align="center">
                      <a  href="materialStoreInEdit?materialSeriesNo=${materialStore.id}">
                        <i class="glyphicon glyphicon-info-sign"></i>
                        入库</a>
                    </td>
                    <td align="center">
                      <a  href="materialStoreOutEdit?materialSeriesNo=${materialStore.id}">
                        <i class="glyphicon glyphicon-info-sign"></i>
                        出库</a>
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

<%--  </div>
</div>--%>
</body>
</html>
