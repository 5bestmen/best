<%--
  Created by IntelliJ IDEA.
  User: ChengXi
  Date: 2015/9/7
  Time: 10:14
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
  <meta charset="utf-8">
  <title>${sessionScope.APP_NAME}-交流系统</title>

  <script src="bower_components/jquery/dist/jquery.min.js"></script>
  <script src="bower_components/bootstrap/dist/js/bootstrap.min.js"></script>
  <script src="bower_components/metisMenu/dist/metisMenu.min.js"></script>
  <script src="dist/js/sb-admin-2.js"></script>

  <link href="bower_components/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">
  <link href="bower_components/metisMenu/dist/metisMenu.min.css" rel="stylesheet">
  <link href="dist/css/sb-admin-2.css" rel="stylesheet">
  <link href="bower_components/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
  <link href="dist/css/style.css" rel="stylesheet" type="text/css">

</head>
<body>

<div id="wrapper">
  <jsp:include page="meun.jsp"/>

  <div id="page-wrapper">

    <div class="row">
      <div class="col-lg-12 col-md-12">
        <div id="svgdiv" class="panel panel-primary" id="map-0" style="height: 1200px">
          <embed id="svgEmbed" src="svg/系统站/电源点接线图.svg" type="image/svg+xml"></embed>
        </div>
      </div>
    </div>
  </div>
</div>
</body>
</html>
