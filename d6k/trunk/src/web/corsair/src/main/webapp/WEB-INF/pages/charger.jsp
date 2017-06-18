<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/8/24
  Time: 15:02
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@ taglib uri="http://corsair.com" prefix="corsair" %>
<html>
<head>
  <meta charset="UTF-8">
  <title>充电机页面</title>

  <link href="bower_components/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">
  <link href="bower_components/metisMenu/dist/metisMenu.min.css" rel="stylesheet">
  <link href="dist/css/sb-admin-2.css" rel="stylesheet">
  <link href="bower_components/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
  <link href="dist/css/style.css" rel="stylesheet" type="text/css">
  <script src="dist/js/html5shiv.min.js"></script>
  <script src="dist/js/respond.min.js"></script>

</head>
<body>
<script src="bower_components/jquery/dist/jquery.min.js"></script>
<script src="bower_components/bootstrap/dist/js/bootstrap.min.js"></script>
<script src="bower_components/metisMenu/dist/metisMenu.min.js"></script>
<script src="dist/js/sb-admin-2.js"></script>
<script src="dist/js/echarts/echarts-all.js"></script>
<script src="dist/js/generalChart.js"></script>
<script src="dist/js/generalMeasurement.js"></script>
<script>
  var echartsOption = {
    tooltip : {
      formatter: "{a} <br/>{c} {b}"
    },
    series : [
      {
        name:'输出电压',
        type:'gauge',
        z: 3,
        min:0,
        max:50,
        radius : '75%',
        center : ['25%', '55%'],    // 默认全局居中
        axisLine: {            // 坐标轴线
          lineStyle: {       // 属性lineStyle控制线条样式
            width: 6
          }
        },
        axisTick: {            // 坐标轴小标记
          length :11,        // 属性length控制线长
          lineStyle: {       // 属性lineStyle控制线条样式
            color: 'auto'
          }
        },
        splitLine: {           // 分隔线
          length :15,         // 属性length控制线长
          lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
            color: 'auto'
          }
        },
        title : {
          textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
            fontWeight: 'bolder',
            fontSize: 13,
            fontStyle: 'italic'
          }
        },
        detail : {
          textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
            fontWeight: 'bolder'
          }
        },
        data:[{value: 40, name: '输出电压 V'}]
      },
      {
        name:'输出电流',
        type:'gauge',
        z: 3,
        min:0,
        max:50,
        radius : '75%',
        center : ['75%', '55%'],    // 默认全局居中
        axisLine: {            // 坐标轴线
          lineStyle: {       // 属性lineStyle控制线条样式
            width: 6
          }
        },
        axisTick: {            // 坐标轴小标记
          length :11,        // 属性length控制线长
          lineStyle: {       // 属性lineStyle控制线条样式
            color: 'auto'
          }
        },
        splitLine: {           // 分隔线
          length :15,         // 属性length控制线长
          lineStyle: {       // 属性lineStyle（详见lineStyle）控制线条样式
            color: 'auto'
          }
        },
        title : {
          textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
            fontWeight: 'bolder',
            fontSize: 13,
            fontStyle: 'italic'
          }
        },
        detail : {
          textStyle: {       // 其余属性默认使用全局文本样式，详见TEXTSTYLE
            fontWeight: 'bolder'
          }
        },
        data:[{value: 20, name: '输出电流 A'}]

      }
    ]
  };

  var option = {
    ajax: true,
    URL: '/StationServlet',
    robin: true,
    timeSpan: 1000,
    getConfig: function(){return {type:'RealMeasurement'};},
    formatNumber: true,
    EchartsOption: echartsOption,
    chartContainer: 'realMeasurement'
  };
  $(function () {
    var chart = $.fn.GeneralChart.init(option);
    chart.load();
  });
</script>
<script>
  function Measurement()
  {
    var meaObject = $.fn.GeneralMeasurement.init();
    meaObject.load();
  }
  window.onload=function(){Measurement();}
</script>

<div id="wrapper">
  <jsp:include page="meun.jsp"/>
  <div id="page-wrapper">
    <div class="row">
      <div class="col-lg-12">
        <h3>充电机</h3>
      </div>
    </div>
    <div class="row">
      <div class="col-lg-7">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-bar-chart-o fa-fw"></i>充电机数据
          </div>
          <div class="panel-body">
            <div class="row">
              <div class="col-lg-5">
                <img class="img-responsive" src="img/charger.png">
              </div>
              <div class="col-lg-7">
                <div class="tab-content">
                    <div id="realMeasurement" class="general-chart"></div>
                </div>
              </div>
            </div>
            <div class="row">
              <div class="col-lg-5">
                <div class="widget-state navy-bg">
                  <div class="text-center">
                    <h5 class="font-bold YX">运行状态</h5>
                  </div>
                </div>
              </div>
              <div class="col-lg-7">
                <div class="row">
                  <div class="col-lg-3">
                    <div class="widget-state navy-bg">
                      <div class="text-center">
                        <h5 class="font-bold YX">电池一</h5>
                      </div>
                    </div>
                  </div>
                  <div class="col-lg-3">
                    <div class="widget-state navy-bg">
                      <div class="text-center">
                        <h5 class="font-bold YX">电池二</h5>
                      </div>
                    </div>
                  </div>
                  <div class="col-lg-3">
                    <div class="widget-state navy-bg">
                      <div class="text-center">
                        <h5 class="font-bold YX">电池三</h5>
                      </div>
                    </div>
                  </div>
                  <div class="col-lg-3">
                    <div class="widget-state navy-bg">
                      <div class="text-center">
                        <h5 class="font-bold YX">电池四</h5>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
              <div class="col-lg-12">
                <div class="row">
                  <div class="col-lg-4 col-md-6">
                    <ul class="list-group clear-list">
                      <corsair:measurement displayAreaName="charger-yc1" PSRID="0051qc34hu506n"/>
                      <%--<li class="list-group-item fist-item">--%>
                        <%--<span class="pull-right"><em>1000</em> <i>V</i></span>--%>
                        <%--<span class="label label-success">current</span>A相交流电压--%>
                      <%--</li>--%>
                      <%--<li class="list-group-item">--%>
                        <%--<span class="pull-right"><em>1000</em> <i>A</i></span>--%>
                        <%--<span class="label label-success">current</span>电流--%>
                      <%--</li>--%>
                      <%--<li class="list-group-item">--%>
                        <%--<span class="pull-right"><em>1000</em> <i>A</i></span>--%>
                        <%--<span class="label label-success">current</span>电流--%>
                      <%--</li>--%>
                    </ul>
                  </div>
                  <div class="col-lg-4 col-md-6">
                    <ul class="list-group clear-list">
                      <corsair:measurement displayAreaName="charger-yc2" PSRID="0051qc34hu506n"/>
                    </ul>
                  </div>
                  <div class="col-lg-4 col-md-6">
                    <ul class="list-group clear-list">
                      <corsair:measurement displayAreaName="charger-yc3" PSRID="0051qc34hu506n"/>
                    </ul>
                  </div>
                </div>
              </div>
              <div class="col-lg-12">
                <div class="row">
                  <corsair:measurement displayAreaName="charger-yx" PSRID="0051qc34hu506n"/>
                  <%--<div class="col-lg-3 col-md-6">--%>
                    <%--<div class="widget-state navy-bg measurement" data-PSRID="DATA_PSR_ID" data-templateIndex="DATA_TEMPLATE_INDEX" data-measurementID="DATA_MEANS_ID" data-measurementType="DATA_MEANS_TYPE" data-magic="DATA_MAGIC_STRING">--%>
                      <%--<div class="text-center">--%>
                        <%--<h5 class="font-bold YX">交流输入过压</h5>--%>
                      <%--</div>--%>
                    <%--</div>--%>
                  <%--</div>--%>
                </div>
              </div>

          </div>
        </div>
      </div>
      <div class="col-lg-5">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-bar-chart-o fa-fw"></i>BMS数据
          </div>
          <div class="panel-body">
            <div class="row">
              <div class="col-lg-12">
                <img class="img-responsive" src="img/battery.png">
              </div>
            </div>
            <div>
              <div class="row">
                <div class="col-lg-6 col-md-6">
                  <ul class="list-group clear-list">
                    <corsair:measurement displayAreaName="battery-yc1" PSRID="0051qc34hgu4h6"/>
                    <%--<li class="list-group-item fist-item">--%>
                      <%--<span class="pull-right"><em>1000</em> <i>A</i></span>--%>
                      <%--<span class="label label-success">current</span>电流--%>
                    <%--</li>--%>
                    <%--<li class="list-group-item">--%>
                      <%--<span class="pull-right"><em>1000</em> <i>A</i></span>--%>
                      <%--<span class="label label-success">current</span>电流--%>
                    <%--</li>--%>
                    <%--<li class="list-group-item">--%>
                      <%--<span class="pull-right"><em>1000</em> <i>A</i></span>--%>
                      <%--<span class="label label-success">current</span>电流--%>
                    <%--</li>--%>
                  </ul>
                </div>
                <div class="col-lg-6 col-md-6">
                  <ul class="list-group clear-list">
                    <corsair:measurement displayAreaName="battery-yc2" PSRID="0051qc34hgu4h6"/>
                  </ul>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>
</body>
</html>
