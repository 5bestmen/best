<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://corsair.com" prefix="corsair"%>
<html>
<head>
    <title>等效利用小时</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link href="resources/css/bootstrap.min.css" rel="stylesheet">
    <link href="resources/css/metisMenu.min.css" rel="stylesheet">
    <link href="resources/css/sb-admin-2.css" rel="stylesheet">
    <link href="resources/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="resources/css/style.css" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/commonStyle.css"/>" rel="stylesheet" type="text/css">

    <script src="resources/dist/js/html5shiv.min.js"></script>
    <script src="resources/dist/js/respond.min.js"></script>
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
        var monthEcOption = {
            title : {
                textStyle: {
                    color:'white'
                },
                text: '月等效利用小时数'
            },
            toolbox: {
                show : true,
                feature : {
                    magicType : {show: true, type: ['line', 'bar']},
                    restore : {show: true}
                }
            },
            tooltip: {
                trigger: 'axis'
            },
            calculable : true,
            xAxis : [
                {
                    type : 'category',
                    axisLabel: {
                        textStyle: {
                            color:'white'
                        }
                    },
                    data : []
                }
            ],
            yAxis : [
                {
                    type : 'value',
                    axisLabel : {
                        formatter: '{value} 小时',
                        axisLabel: {
                            textStyle: {
                                color:'white'
                            }
                        },
                    }
                }
            ],
            series: [
                {
                    name: '等效利用小时数',
                    type: 'bar',
                    smooth: true,
                    itemStyle: {
                        normal: {
                            color: '#32d2c9',
                            label: {show: true}
                        }
                    },
                    data: [],
                    markPoint : {
                        data : [
                            {type : 'max', name: '最大值'},
                            {type : 'min', name: '最小值'}
                        ]
                    },
                    markLine : {
                        data : [
                            {type : 'average', name: '平均值'}
                        ]
                    }
                }
            ],
            grid : {
                x2: '5%',
                y2: '10%'
            }
        };

        var monthOption = {
            ajax: true,
            URL: '/api/statistic',
            robin: false,
            timeSpan: 10000,
            getConfig: function(){return {type:'monthStatistic'};},
            formatNumber: true,
            EchartsOption: monthEcOption,
            chartContainer: 'monthStatistic'
        };

        var yearEcOption = {
            title : {
                textStyle: {
                    color:'white'
                },
                text: '年等效利用小时数'
            },
            toolbox: {
                show : true,
                feature : {
                    magicType : {show: true, type: ['line', 'bar']},
                    restore : {show: true}
                }
            },
            tooltip: {
                trigger: 'axis'
            },
            calculable : true,
            xAxis : [
                {
                    type : 'category',
//                boundaryGap : false,
                    axisLabel: {
                        textStyle: {
                            color:'white'
                        }
                    },
                    data : []
                }
            ],
            yAxis : [
                {
                    type : 'value',
                    axisLabel : {
                        formatter: '{value} 小时',
                        textStyle: {
                            color:'white'
                        }
                    }
                }
            ],
            series : [
                {
                    name: '等效利用小时数',
                    type: 'bar',
                    smooth:true,
                    itemStyle: {
                        normal: {
                            color: '#32d2c9',
                            label: {show: true}
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

        var yearOption = {
            ajax: true,
            URL: '/api/statistic',
            robin: true,
            timeSpan: 10000,
            getConfig: function(){return {type:'yearStatistic'};},
            formatNumber: true,
            EchartsOption: yearEcOption,
            chartContainer: 'yearStatistic'
        };

        var totalEcOption = {
            title : {
                textStyle: {
                    color:'white'
                },
                text: '累计等效利用小时数'
            },
            toolbox: {
                show : true,
                feature : {
                    magicType : {show: true, type: ['line', 'bar']},
                    restore : {show: true}
                }
            },
            tooltip: {
                trigger: 'axis'
            },
            calculable : true,
            xAxis : [
                {
                    type : 'category',
//                boundaryGap : false,
                    axisLabel: {
                        textStyle: {
                            color:'white'
                        }
                    },
                    data : []
                }
            ],
            yAxis : [
                {
                    type : 'value',
                    axisLabel : {
                        formatter: '{value} 小时',
                        textStyle: {
                            color:'white'
                        }
                    }
                }
            ],
            series : [
                {
                    name: '等效利用小时数',
                    type: 'bar',
                    smooth:true,
                    itemStyle: {
                        normal: {
                            color: '#32d2c9',
                            label: {show: true}
                        }
                    },
                    data: [],
                    markPoint : {
                        data : [
                            {type : 'max', name: '最大值'},
                            {type : 'min', name: '最小值'}
                        ]
                    }
                }
            ],
            grid : {
                x2: '5%',
                y2: '10%'
            }
        };

        var totalOption = {
            ajax: true,
            URL: '/api/statistic',
            robin: true,
            timeSpan: 10000,
            getConfig: function(){return {type:'totalStatistic'};},
            formatNumber: true,
            EchartsOption: totalEcOption,
            chartContainer: 'totalStatistic'
        };

        $(function () {

            var totalChart = $.fn.GeneralChart.init(totalOption);
            totalChart.load();

            $("#hours-tab-3").removeClass("active");
            $("#hours-tab-2").addClass("active");

            var yearChart = $.fn.GeneralChart.init(yearOption);
            yearChart.load();

            $("#hours-tab-2").removeClass("active");
            $("#hours-tab-1").addClass("active");

            var monthChart = $.fn.GeneralChart.init(monthOption);
            monthChart.load();
        });

    </script>

    <%--<div id="wrapper">
      <jsp:include page="meun.jsp"/>
      &lt;%&ndash;todo 工作空间，实际的内容将在这个位置添加&ndash;%&gt;
      <div id="page-wrapper">--%>
        <div class="row">
          <div class="col-lg-12">
            <h3 class="page-header">等效利用小时</h3>
              <div class="col-lg-12">
                  <div id="monthStatistic" class="general-chart">
                  </div>
              </div>
              <div class="col-lg-12">
                  <div id="yearStatistic" class="general-chart">

                  </div>
              </div>
              <div class="col-lg-12">
                  <div id="totalStatistic" class="general-chart">

                  </div>
              </div>
          </div>
        </div>
        <%--<div class="row">--%>
            <%--<div class="col-lg-12">--%>
                <%--<div class="panel panel-info">--%>
                    <%--<div class="panel-options lightblueheader">--%>
                        <%--<ul class="nav nav-tabs">--%>
                            <%--<li><a data-toggle="tab" href="#hours-tab-1">按月统计</a>--%>
                            <%--</li>--%>
                            <%--<li><a data-toggle="tab" href="#hours-tab-2">按年统计</a>--%>
                            <%--</li>--%>
                            <%--<li><a data-toggle="tab" href="#hours-tab-3">累计</a>--%>
                            <%--</li>--%>
                        <%--</ul>--%>
                    <%--</div>--%>

                    <%--<div class="panel-body">--%>
                        <%--<div class="tab-content">--%>
                            <%--<div id="hours-tab-1" class="tab-pane">--%>
                                <%--<div id="monthStatistic" class="general-chart">--%>
                                <%--</div>--%>
                            <%--</div>--%>

                            <%--<div id="hours-tab-2" class="tab-pane">--%>
                                <%--<div id="yearStatistic" class="general-chart">--%>

                                <%--</div>--%>
                            <%--</div>--%>
                            <%--<div id="hours-tab-3" class="tab-pane active">--%>
                                <%--<div id="totalStatistic" class="general-chart">--%>

                                <%--</div>--%>
                            <%--</div>--%>
                        <%--</div>--%>

                    <%--</div>--%>
                <%--</div>--%>
            <%--</div>--%>
        <%--</div>--%>
     <%-- </div>
    </div>--%>
</body>
</html>
