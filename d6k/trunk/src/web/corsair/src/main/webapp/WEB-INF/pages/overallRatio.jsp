<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%--<%@ taglib prefix="form" uri="http://struts.apache.org/tags-html" %>--%>
<%@ taglib prefix="form" uri="http://www.springframework.org/tags/form" %>
<html>
<head>
    <title>系统综合效率</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <link href="<c:url value="/resources/css/bootstrap.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/metisMenu.min.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/sb-admin-2.css"/>" rel="stylesheet">
    <link href="<c:url value="/resources/css/font-awesome.min.css"/>" rel="stylesheet" type="text/css">
    <link href="<c:url value="/resources/css/style.css"/>" rel="stylesheet" type="text/css">

    <script src="<c:url value="/resources/dist/js/html5shiv.min.js"/>"></script>
    <script src="<c:url value="/resources/dist/js/respond.min.js"/>"></script>
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

    var monthEcOption = {
        title : {
            textStyle: {
                color:'white'
            },
            text: '月综合效率'
        },
        toolbox: {
            show: true,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
        },
        tooltip: {
            trigger: 'axis'
        },
        calculable: true,
        xAxis: [
            {
                type: 'category',
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                type: 'value',
                axisLabel: {
                    formatter: '{value} %',
                    textStyle: {
                        color:'white'
                    }
                }
            }
        ],
        series: [
            {
                name: '综合效率',
                type: 'line',
                smooth: true,
                itemStyle: {
                    normal: {
                        areaStyle: {
                            color: '#32d2c9',
                            type: 'default'
                        },
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
        grid: {
            x2: '5%',
            y2: '10%'
        }
    };

    var monthOption = {
        ajax: true,
        URL: '/api/statistic',
        robin: false,
        timeSpan: 10000,
        getConfig: function () {
            return {type: 'monthOverallRatio'};
        },
        formatNumber: true,
        EchartsOption: monthEcOption,
        chartContainer: 'monthRatio'
    };

    var yearEcOption = {
        title : {
            textStyle: {
                color:'white'
            },
            text: '年综合效率'
        },
        toolbox: {
            show: true,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
        },
        tooltip: {
            trigger: 'axis'
        },
        calculable: true,
        xAxis: [
            {
                type: 'category',
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                type: 'value',
                axisLabel: {
                    formatter: '{value} %',
                    textStyle: {
                        color:'white'
                    }
                }
            }
        ],
        series: [
            {
                name: '综合效率',
                type: 'bar',
                smooth: true,
                itemStyle: {
                    normal: {
                        color: '#32d2c9',
                        areaStyle: {
                            type: 'default'
                        },
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
        grid: {
            x2: '5%',
            y2: '10%'
        }
    };

    var yearOption = {
        ajax: true,
        URL: '/api/statistic',
        robin: false,
        timeSpan: 10000,
        getConfig: function () {
            return {type: 'yearOverallRatio'};
        },
        formatNumber: true,
        EchartsOption: yearEcOption,
        chartContainer: 'yearRatio'
    };

    var totalEcOption = {
        title : {
            textStyle: {
                color:'white'
            },
            text: '累计综合效率'
        },
        toolbox: {
            show: true,
            feature: {
                magicType: {show: true, type: ['line', 'bar']},
                restore: {show: true}
            }
        },
        tooltip: {
            trigger: 'axis'
        },
        calculable: true,
        xAxis: [
            {
                type: 'category',
                axisLabel : {
                    textStyle: {
                        color:'white'
                    }
                },
                data: []
            }
        ],
        yAxis: [
            {
                type: 'value',
                axisLabel: {
                    formatter: '{value} %',
                    textStyle: {
                        color:'white'
                    }
                }
            }
        ],
        series: [
            {
                name: '综合效率',
                type: 'bar',
                smooth: true,
                itemStyle: {
                    normal: {
                        color: '#32d2c9',
                        areaStyle: {
                            type: 'default'
                        },
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
        grid: {
            x2: '5%',
            y2: '10%'
        }
    };

    var totalOption = {
        ajax: true,
        URL: '/api/statistic',
        robin: false,
        timeSpan: 10000,
        getConfig: function () {
            return {type: 'totalOverallRatio'};
        },
        formatNumber: true,
        EchartsOption: totalEcOption,
        chartContainer: 'totalRatio'
    };

    $(function () {

        var totalChart = $.fn.GeneralChart.init(totalOption);
        totalChart.load();

        var yearChart = $.fn.GeneralChart.init(yearOption);
        yearChart.load();

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
                <h3 class="page-header">综合效率</h3>
            </div>
            <div class="col-lg-12">
                <div id="monthRatio" class="general-chart"></div>
            </div>
            <div class="col-lg-12">
                <div id="yearRatio" class="general-chart"></div>
            </div>
            <div class="col-lg-12">
                <div id="totalRatio" class="general-chart"></div>
            </div>
        </div>
        <%--<div class="row">--%>
        <%--<div id="monthRatio" class="general-chart">--%>
        <%--</div>--%>
        <%--<div id="yearRatio" class="general-chart"></div>--%>
        <%--<div id="totalRatio" class="general-chart">--%>
        <%--</div>--%>
        <%--<div class="col-lg-12">--%>
        <%--<div class="panel panel-info">--%>
        <%--<div class="panel-options lightblueheader">--%>
        <%--<ul class="nav nav-tabs">--%>
        <%--<li><a data-toggle="tab" href="#ratio-tab-1">日综合效率</a>--%>
        <%--</li>--%>
        <%--<li><a data-toggle="tab" href="#ratio-tab-2">月综合效率</a>--%>
        <%--</li>--%>
        <%--<li><a data-toggle="tab" href="#ratio-tab-3">年综合效率</a>--%>
        <%--</li>--%>
        <%--</ul>--%>
        <%--</div>--%>
        <%--<div class="panel-body">--%>
        <%--<div class="tab-content">--%>
        <%--<div id="ratio-tab-1" class="tab-pane">--%>
        <%--<div id="monthRatio" class="general-chart">--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--<div id="ratio-tab-2" class="tab-pane">--%>
        <%--<div id="yearRatio" class="general-chart">--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--<div id="ratio-tab-3" class="tab-pane active">--%>
        <%--<div id="totalRatio" class="general-chart">--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
        <%--</div>--%>
    <%--</div>
</div>--%>
</body>
</html>
