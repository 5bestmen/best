<%@ page import="com.corsair.service.ApplicationInformationService" %>
<%@ page import="org.springframework.web.context.support.WebApplicationContextUtils" %>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<%
  session.getAttributeNames();

  ApplicationInformationService service = (ApplicationInformationService) WebApplicationContextUtils
          .getRequiredWebApplicationContext(session.getServletContext()).getBean("applicationInformationService");

  String point_lng = service.getCenterPoint_lng();
  String point_lat = service.getCenterPoint_lat();
  Float zoom = service.getMapZoom();
  boolean zoomable = service.isZoomable();
  boolean dragable = service.isDragable();

%>
<!DOCTYPE html>
<html lang="zh_CN">

<head>

  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">

  <title>${sessionScope.APP_NAME}-主页</title>

  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

  <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
  <link href="<c:url value="/resources/dist/js/html5shiv.min.js"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/dist/js/respond.min.js"/>" rel="stylesheet" type="text/css">
  <!--<script src="dist/js/html5shiv.min.js"></script>-->
  <!--<script src="dist/js/respond.min.js"></script>-->
  <![endif]-->

  <%--todo 页面自身需要的CSS begin--%>
  <%--页面自身需要的CSS end--%>
</head>

<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src="<c:url value="http://webapi.amap.com/maps?v=1.3&key=6d0f573ad98e3b060ac5e53e65d2ac05"/>"></script>

<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script src='http://webapi.amap.com/maps?v=1.3&key=6d0f573ad98e3b060ac5e53e65d2ac05'></script>

<script>
  $(function() {
    var meaObject = $.fn.GeneralMeasurement.init();
    meaObject.load();
  });
</script>


<div id="wrapper">
  <jsp:include page="meun.jsp"/>


  <%--todo 工作空间，实际的内容将在这个位置添加--%>
  <div id="page-wrapper" >
    <div class="row">
      <div class="col-lg-12">
        <h3>主面板
          <span class="alignright">${sessionScope.APP_NAME}</span>
        </h3>
      </div>
    </div>
    <div id="mapContainer" style="height: 800px">

    </div>
  </div>
  <div class="top-left-panel white-text">
    <div class="text-center">
      <h5>概览</h5>
    </div>
    <table class="table table-responsive">
      <tbody>
      <corsair:measurement displayAreaName="index-overview" PSRID="totalview"></corsair:measurement>
      </tbody>
    </table>
  </div>
  <div class="middle-left-panel white-text">
    <div class="text-center">
      <h5>节能减排</h5>
    </div>
    <table class="table table-responsive">
      <tbody>
        <corsair:measurement displayAreaName="index-concre" PSRID="totalcere"/>
      </tbody>
    </table>
  </div>
  <div class="bottom-panel" id="dayPower">

  </div>

</div>

<script>
  var p = JSON.parse('${locationinfo}');
  console.log(p);


  var map = new AMap.Map('mapContainer', {
    center: [<%=point_lng%>, <%=point_lat%>],
    zoom: <%=zoom%>,
    zoomEnable:<%=zoomable%>,
    dragEnable:<%=dragable%>
  });

  for (var i in p) {
    addMarker(p[i], map);
  }

  function addMarker(pos, mapObject){
    var m = document.createElement("div");
    m.className = "marker";
    var n = document.createElement("div");
    n.innerHTML = pos.station_name;
    m.appendChild(n);

    var marker = new AMap.Marker({
      map:mapObject,
      icon:"http://webapi.amap.com/images/marker_sprite.png",
      position:[pos.longitude, pos.latitude],
      content:m
//      offset:new AMap.Pixel(30, 0)
    });

//    marker.setLabel({//label的父div默认蓝框白底右下角显示，样式className为：amap-marker-label
//      offset:new AMap.Pixel(30, 0),//修改父div相对于maker的位置
//      content:pos.name
//    });

    AMap.event.addListener(marker, 'click', function() {
      showInfoWindow(pos, mapObject);
    });
  }

  //在指定位置打开信息窗体
  function showInfoWindow(data, mapObject) {
    var info = [];
    info.push("<div><div><a " + "href= 'station?stationID=" + data.linkurl + "'>" + data.station_name +
            "</a></div> ");
    info.push("<div style=\"padding:0px 0px 0px 4px;\">地址：" +
            data.station_city);
    info.push("实时功率：" +
            (Math.random() * 10).toFixed(2) + ' MW');
    info.push("日发电量：" +
            (Math.random() * 20).toFixed(2) + ' MWh' +
            "</div></div>");

    infoWindow = new AMap.InfoWindow({
      content: info.join("<br/>"),  //使用默认信息窗体框样式，显示信息内容
      offset: new AMap.Pixel(16, -45),
      closeWhenClickMap: true
    });

    infoWindow.open(mapObject, [data.longitude, data.latitude]);
  }
</script>

<script>
  var dayEcOption = {
    backgroundColor:'rgba(45, 94, 124, 0.8)',
    title:{
      show:true,
      text:'日上网发电量',
      x:'center',
      y:'top'
    },
    toolbox: {
      show : true,
      feature : {
        magicType : {show: true, type: ['line', 'bar']},
        restore : {show: true}
      }
    },
    calculable : true,
    xAxis : [
      {
        type : 'category',
        data : []
      }
    ],
    yAxis : [
      {
        type : 'value',
        name:'发电量',
        axisLabel : {
          formatter: '{value} MWh'
        }
      },
      {
        type : 'value',
        name:'等效小时',
        axisLabel : {
          formatter: '{value} h'
        }
      }
    ],
    series : [
      {
        name: '发电量',
        type: 'bar',
        smooth:true,
        itemStyle:{
          normal:
          {
            label : {show: true}
          }
        },
        data: [],
        markPoint : {
          data : [
            {type : 'max', name: '最大值'},
            {type : 'min', name: '最小值'}
          ]
        }
      },
      {
        name: '等效小时',
        type: 'line',
        smooth:true,
        yAxisIndex: 1,
        itemStyle:{
          normal:
          {
            label : {show: true}
          }
        },
        data: []
      }
    ],
    grid : {
      x2: '5%',
      y2: '10%'
    }
  };

  var optionDayEnergyCurve = {
    ajax:true,
    URL:'/dayEnergyChartServlet',
    robin:true,
    timeSpan:10000,
    getConfig:function(){return {type:'DayPowerCurve'};},
    formatNumber: true,
    EchartsOption: dayEcOption,
    chartContainer: 'dayPower'
  }

  $(function () {

    var curveChart = $.fn.GeneralChart.init(optionDayEnergyCurve);
    curveChart.load();
  });

</script>

</body>

</html>
