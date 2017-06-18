<%--
  Created by IntelliJ IDEA.
  User: libh
  Date: 2015/8/29
  Time: 12:52
  To change this template use File | Settings | File Templates.
--%>
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<%
  session.getAttributeNames();
%>
<html>
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta name="description" content="">
  <meta name="author" content="">

  <title>${sessionScope.APP_NAME}-辅助决策</title>

  <link href="resources/css/bootstrap.min.css" rel="stylesheet">
  <link href="resources/css/metisMenu.min.css" rel="stylesheet">
  <link href="resources/css/sb-admin-2.css" rel="stylesheet">
  <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
  <link href="resources/css/style.css" rel="stylesheet" type="text/css">

  <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
  <script src="resources/dist/js/html5shiv.min.js"></script>
  <script src="resources/dist/js/respond.min.js"></script>
  <![endif]-->
</head>
<body>
<script src="resources/js/jquery.min.js"></script>
<script src="resources/js/bootstrap.min.js"></script>
<script src="resources/js/metisMenu.js"></script>
<script src="resources/js/sb-admin-2.js"></script>
<script src="resources/dist/js/echarts/echarts-all.js"></script>
<script src="resources/js/generalChart.js"></script>
<script src="resources/js/generalMeasurement.js"></script>

<script>
  var gbOption = {
    legend: {
      data:['发电量']
    },
    toolbox: {
      show : true,
      feature : {
        magicType : {show: true, type: ['line', 'bar']},
        restore : {show: true}
      }
    },
    calculable : true,
    xAxis: [
      {
        type: 'category',
        splitLine: {show: false},
        data: ['达板城电站','米东电站','沙依巴克电站','天山电站','头屯河电站','新市电站']
//        axisLabel:{
//          rotate:40
////          textStyle:{
////            color:"red",
////            fontSize:16
////          }
//        }

      }
    ],
    yAxis: [
      {
        type: 'value',
        axisLabel: {
          formatter: '{value} kWh'
        }
      }
    ],
    series: [
      {
        name: '发电量',
        type: 'bar',
        stack: '总量',
//        smooth:true,
        barWidth:50,
        itemStyle: {normal: {label: {show: true, position: 'top'}}},
//        itemStyle: {normal: {color: '#1ab394', label: {show: true, position: 'top'}}},
        data: []
      }
    ],
    grid : {
      x2: '5%',
      y2: '10%'
    }
  };

  var  capacityOption= {
    ajax: true,
    URL: '/StationServlet',
    robin: true,
    timeSpan: 10000,
    getConfig: function(){return {type:'co2'};},
    formatNumber: true,
    EchartsOption: gbOption,
    chartContainer: 'generateCapacity'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(capacityOption);
    chart.load();
  });
</script>
<script>
  var gcOption = {
    legend: {
      data:['发电效益']
    },
    toolbox: {
      show : true,
      feature : {
        magicType : {show: true, type: ['line', 'bar']},
        restore : {show: true}
      }
    },
    calculable : true,
    xAxis: [
      {
        type: 'category',
        splitLine: {show: false},
        data: ['达板城电站','米东电站','沙依巴克电站','天山电站','头屯河电站','新市电站']
      }
    ],
    yAxis: [
      {
        type: 'value',
        axisLabel: {
          formatter: '{value} 万元'
        }
      }
    ],
    series: [
      {
        name: '发电效益',
        type: 'bar',
        stack: '总量',
        barWidth:50,
        itemStyle: {normal: {label: {show: true, position: 'top'}}},
        data: []
      }
    ],
    grid : {
      x2: '5%',
      y2: '10%'
    }
  };

  var  benefitOption= {
    ajax: true,
    URL: '/StationServlet',
    robin: true,
    timeSpan: 10000,
    getConfig: function(){return {type:'co2'};},
    formatNumber: true,
    EchartsOption: gcOption,
    chartContainer: 'generateBenefit'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(benefitOption);
    chart.load();
  });
</script>
<script>
  var ghOption = {
    legend: {
      data:['发电小时数']
    },
    toolbox: {
      show : true,
      feature : {
        magicType : {show: true, type: ['line', 'bar']},
        restore : {show: true}
      }
    },
    calculable : true,
    xAxis: [
      {
        type: 'category',
        splitLine: {show: false},
        data: ['达板城电站','米东电站','沙依巴克电站','天山电站','头屯河电站','新市电站']
      }
    ],
    yAxis: [
      {
        type: 'value',
        axisLabel: {
          formatter: '{value} h'
        }
      }
    ],
    series: [
      {
        name: '发电小时数',
        type: 'bar',
        stack: '总量',
        barWidth:50,
        itemStyle: {normal: {label: {show: true, position: 'top'}}},
        data: []
      }
    ],
    grid : {
      x2: '5%',
      y2: '10%'
    }
  };

  var  hoursOption= {
    ajax: true,
    URL: '/StationServlet',
    robin: true,
    timeSpan: 10000,
    getConfig: function(){return {type:'co2'};},
    formatNumber: true,
    EchartsOption: ghOption,
    chartContainer: 'generateHours'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(hoursOption);
    chart.load();
  });
</script>
<script>
  var labelTop = {
    normal : {
      label : {
        show : true,
        position : 'center',
        formatter : '{b}',
        textStyle: {
          baseline : 'bottom'
        }
      },
      labelLine : {
        show : false
      }
    }
  };
  var labelFromatter = {
    normal : {
      label : {
        formatter : function (params){
          return 100 - params.value + '%'
        },
        textStyle: {
          baseline : 'top'
        }
      }
    }
  }
  var labelBottom = {
    normal : {
      color: '#ccc',
      label : {
        show : true,
        position : 'center'
      },
      labelLine : {
        show : false
      }
    },
    emphasis: {
      color: 'rgba(0,0,0,0)'
    }
  };
  var radius = [30, 35];
  var inOption = {
    legend: {
      x : 'center',
      y : '50%',
      data:[
        '达板城1','达板城2','米东1','米东2','沙依巴克1','沙依巴克2','天山1','天山2','头屯河1','头屯河2','新市1','新市2'
      ]
    },
    title : {
      text: '逆变器转换效率',
      x: 'center'
    },
    series : [
      {
        type : 'pie',
        center : ['10%', '30%'],
        radius : radius,
        x: '0%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'达板城1', value:94,itemStyle : labelTop},
          {name:'other', value:6, itemStyle : labelBottom}

        ]
      },
      {
        type : 'pie',
        center : ['26%', '30%'],
        radius : radius,
        x:'20%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:6, itemStyle : labelBottom},
          {name:'达板城2', value:94,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['42%', '30%'],
        radius : radius,
        x:'40%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:5, itemStyle : labelBottom},
          {name:'米东1', value:95,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['58%', '30%'],
        radius : radius,
        x:'60%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:10, itemStyle : labelBottom},
          {name:'米东2', value:90,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['74%', '30%'],
        radius : radius,
        x:'80%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:3, itemStyle : labelBottom},
          {name:'沙依巴克1', value:97,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['90%', '30%'],
        radius : radius,
        x:'80%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:3, itemStyle : labelBottom},
          {name:'沙依巴克2', value:97,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['10%', '85%'],
        radius : radius,
        y: '55%',   // for funnel
        x: '0%',    // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:8, itemStyle : labelBottom},
          {name:'天山1', value:92,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['26%', '85%'],
        radius : radius,
        y: '55%',   // for funnel
        x:'20%',    // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:8, itemStyle : labelBottom},
          {name:'天山2', value:92,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['42%', '85%'],
        radius : radius,
        y: '55%',   // for funnel
        x:'40%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:8, itemStyle : labelBottom},
          {name:'头屯河1', value:92,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['58%', '85%'],
        radius : radius,
        y: '55%',   // for funnel
        x:'60%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:3, itemStyle : labelBottom},
          {name:'头屯河2', value:97,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['74%', '85%'],
        radius : radius,
        y: '55%',   // for funnel
        x:'80%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:9, itemStyle : labelBottom},
          {name:'新市1', value:91,itemStyle : labelTop}
        ]
      },
      {
        type : 'pie',
        center : ['90%', '85%'],
        radius : radius,
        y: '55%',   // for funnel
        x:'80%', // for funnel
        itemStyle : labelFromatter,
        data : [
          {name:'other', value:9, itemStyle : labelBottom},
          {name:'新市2', value:91,itemStyle : labelTop}
        ]
      }
    ]
  };

  var  inverterOption= {
    ajax: false,
    URL: '/StationServlet',
    robin: true,
    timeSpan: 10000,
    getConfig: function(){return {type:'inverterEfficiency'};},
    formatNumber: true,
    EchartsOption: inOption,
    chartContainer: 'inverterEfficiency'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(inverterOption);
    chart.load();
  });
</script>
<script>
  var scOption = {
    title : {
      text: '电站设备表现',
      x:'center'
    },
    tooltip : {
      trigger: 'item',
      formatter: "{a} <br/>{b} : {c} ({d}%)"
    },
    legend: {
      orient : 'vertical',
      x : 'left',
      data:[]
    },
    calculable : true,
    series : [
      {
        name:'设备概况',
        type:'pie',
        radius : ['50%', '70%'],
        itemStyle : {
          normal : {
            label : {
              show : false
            },
            labelLine : {
              show : false
            }
          },
          emphasis : {
            label : {
              show : true,
              position : 'center',
              textStyle : {
                fontSize : '16',
                fontWeight : 'bold'
              }
            }
          }
        },
        data:[]
      }
    ]
  };
  var  stationOption= {
    ajax: true,
    URL: '/StationServlet',
    robin: false,
    timeSpan: 10000,
    getConfig: function(){return {type:'stationComplex'};},
    formatNumber: true,
    EchartsOption: scOption,
    chartContainer: 'stationComplex'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(stationOption);
    chart.load();
  });
</script>
<script>
  var ccOption = {
    title : {
      text: '厂商综合表现',
      x:'center'
    },
    tooltip : {
      trigger: 'item',
      formatter: "{a} <br/>{b} : {c} ({d}%)"
    },
    legend: {
      orient : 'vertical',
      x : 'left',
      data:[]
    },
    calculable : true,
    series : [
      {
        name:'设备个数',
        type:'pie',
        radius : '55%',
        center: ['50%', '60%'],
        data:[]
      }
    ]
  };
  var  companyOption= {
    ajax: true,
    URL: '/StationServlet',
    robin: true,
    timeSpan: 10000,
    getConfig: function(){return {type:'companyComplex'};},
    formatNumber: true,
    EchartsOption: ccOption,
    chartContainer: 'companyComplex'
  };

  $(function() {
    var chart = $.fn.GeneralChart.init(companyOption);
    chart.load();
  });
</script>
<%--<div id="wrapper">
  <jsp:include page="meun.jsp"/>

  <div id="page-wrapper">--%>
    <div class="row">
      <div class="col-lg-12">
        <h3>辅助决策</h3>
      </div>
    </div>
    <div class="row">
      <div class="col-lg-6">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-child fa-fw"></i>发电量排名
          </div>
          <div class="panel-body">
            <div id="generateCapacity" class="general-chart"></div>
          </div>
        </div>
      </div>
      <div class="col-lg-6">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-child fa-fw"></i>发电效益排名
          </div>
          <div class="panel-body">
            <div id="generateBenefit" class="general-chart"></div>
          </div>
        </div>
      </div>
      <div class="col-lg-6">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-child fa-fw"></i>发电小时数排名
          </div>
          <div class="panel-body">
            <div id="generateHours" class="general-chart"></div>
          </div>
        </div>
      </div>
      <div class="col-lg-6">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-child fa-fw"></i>逆变器转换效率排名
          </div>
          <div class="panel-body">
            <div id="inverterEfficiency" class="general-chart"></div>
          </div>
        </div>
      </div>
      <div class="col-lg-6">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-child fa-fw"></i>电站综合排名
          </div>
          <div class="panel-body">
            <div id="stationComplex" class="general-chart"></div>
          </div>
        </div>
      </div>
      <div class="col-lg-6">
        <div class="panel panel-primary">
          <div class="panel-heading">
            <i class="fa fa-child fa-fw"></i>厂商综合排名
          </div>
          <div class="panel-body">
            <div id="companyComplex" class="general-chart"></div>
          </div>
        </div>
      </div>
    </div>
  <%--</div>
</div>--%>
</body>
</html>
