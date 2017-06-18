<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/7/28
  Time: 10:01
  To change this template use File | Settings | File Templates.
--%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>

<%
  session.getAttributeNames();
%>
<!DOCTYPE html>
<html lang="zh_CN">
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">

  <title>${sessionScope.APP_NAME}-电站信息</title>

  <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
  <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
  <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">
<style>
  td{
    color: #ffffff;
  }
</style>
  <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
  <script src="/resources/dist/js/html5shiv.min.js"></script>
  <script src="/resources/dist/js/respond.min.js"></script>
  <![endif]-->
</head>
<body>
<script src="<c:url value="/resources/js/jquery.min.js"/>"></script>
<script src="<c:url value="/resources/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/resources/js/metisMenu.js"/>"></script>
<script src="<c:url value="/resources/js/sb-admin-2.js"/>"></script>
<script src="<c:url value="/resources/js/echarts-all.js"/>"></script>
<script src="<c:url value="/resources/js/generalChart.js"/>"></script>
<script src="<c:url value="/resources/js/generalMeasurement.js"/>"></script>
<script>
  var ecOption = {
    tooltip : {
      trigger: 'item',
      formatter: '{b}'
    },
    dataRange: {
      orient: 'horizontal', // 'vertical'
      splitNumber: 0,
      min : 0,
      max : 100,
      show:false,
//    calculable : true,
      color: ['#ff3333', 'orange', 'yellow','lime','aqua']
    },
    series : [
      {
        name: '中国',
        type: 'map',
        mapType: 'china',
        selectedMode : 'multiple',
        itemStyle:{
          normal:{label:{show:true}},
          emphasis:{ borderWidth:2,
            borderColor:'#fff',
            color: '#33bb66',
            label:{show:true}}
        },
        //data:[{name:'浙江', selected:true}],
        data:[{name:'上海', selected:true}],
        geoCoord:{
          //"宁波":[121.33,29.49]
          "临港":[31.2561,121.6928]

        }
      },
      {
        name: '新疆',
        type: 'map',
        mapType: 'china',
        data: [],
        markPoint: {
          symbol: 'emptyCircle',
          symbolSize: function (v) {
            return 10 + v / 100
          },
          effect: {
            show: true,
            shadowBlur: 0
          },
          itemStyle: {
            normal: {
              label: {show: false}

            }
          },
          data: [
            //{name: "宁波", value: 50},
            {name: "临港", value: 1},
          ]
        }
      }
    ]
  };

  var conMapOption = {
    ajax: false,
    URL: '/StationServlet',
    robin: false,
    timeSpan: 10000,
    getConfig: function(){return {type:'co2'};},
    formatNumber: true,
    EchartsOption: ecOption,
    chartContainer: 'co2'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(conMapOption);
    chart.load();
  });

</script>
<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>--%>

  <div >
    <div class="row">
      <div class="col-lg-12">
        <h3>电站信息</h3>
      </div>
    </div>
    <div class="row">
      <div class="col-md-12">
        <div class="panel">
          <div class="panel-body" >
            <div class="row">
              <div class="col-md-8" style="margin-right: -3px;">
                <div class="m-l-xs map-chart-bg">
                  <div id="co2" class="map-chart"></div>
                </div>

              </div>
              <div class="col-md-4" style="background-image: url('img/title.png')">
                <div >
                  <img  class="img-responsive" src="/resources/img/situation.png">
                </div>
                <div >
                  <hr/>
                  <h2><strong>建设概况</strong></h2>

                  <span>本项目位于上海市浦东新区，项目所在地交通便利，地理位置优越，全年日照时数达2120.5小时，太阳辐射总量为5225MJ/m2，是适宜建设太阳能光伏电站的场址。</span>
                  <br/>
                  <span>太阳能电池阵列由636块265Wp多晶硅光伏组件组成，组件均采用组件支架以16°角安装在混凝土屋面。每个6串组件接入一台组串式逆变器，逆变器就近安装在屋顶花架支撑柱上，有2台30kW和3台33kW逆变器。</span>
                  <h4></h4>
                  <table class="table table-responsive">
                    <tbody >
                    <tr>
                      <td>
                        总装机容量
                      </td>
                      <td>
                        5 MWp
                      </td>
                    </tr>
                    <tr>
                      <td>
                        年平均辐射总量
                      </td>
                      <td>
                        5225 MJ/m2
                      </td>
                    </tr>
                    <tr>
                      <td>
                        年平均日照小时数
                      </td>
                      <td>
                        2060 h
                      </td>
                    </tr>
                    <tr>
                      <td>
                        总装机功率
                      </td>
                      <td>
                        5 MWp
                      </td>
                    </tr>
                    <tr>
                      <td>
                        发电系统总效率
                      </td>
                      <td>
                        77.7 %
                      </td>
                    </tr>
                    <tr>
                      <td>
                        年发电量
                      </td>
                      <td>
                        177,911 kWh
                      </td>
                    </tr>
                    </tbody>
                  </table>

                </div>
              </div>
            </div>

          </div>
        </div>
      </div>
    </div>
  </div>
<%--</div>--%>
</body>
</html>
