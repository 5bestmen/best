/**
 * Created by 洪祥 on 15/7/10.
 */
(function ($) {
    String.prototype.format = function (args) {
        var result = this;
        if (arguments.length > 0) {
            if (arguments.length == 1 && typeof (args) == "object") {
                for (var key in args) {
                    if (args[key] != undefined) {
                        var reg = new RegExp("({" + key + "})", "g");
                        result = result.replace(reg, args[key]);
                    }
                }
            }
            else {
                for (var i = 0; i < arguments.length; i++) {
                    if (arguments[i] != undefined) {
                        var reg = new RegExp("({[" + i + "]})", "g");
                        result = result.replace(reg, arguments[i]);
                    }
                }
            }
        }
        return result;
    };

    $.fn.GeneralChart = {
        //初始化方法
        init: function (options) {
            //初始化列参数
            options = $.extend({}, $.fn.GeneralChart, options);

            var chartObject = {
                options: options,
                chart: null,
                interval: null,

                setRobin: function (robin) {
                    var chartObject = this;
                    var chartOptions = chartObject.options;

                    chartOptions.robin = robin;
                },

                load: function () {
                    var chartObject = this;
                    var chartOptions = chartObject.options;

                    //交互式图表情况下，清空图表并关闭计时器
                    if (chartObject.chart != null) {
                        chartObject.chart.clear();

                        if (chartObject.interval != null) {
                            clearInterval(chartObject.interval);
                        }
                    }

                    // 获取div曲线ID后初始化
                    chartObject.chart = echarts.init(document.getElementById(chartOptions.chartContainer));

                    chartObject.chart.showLoading({
                        text: "正在加载...", effect: 'whirling', textStyle: {fontSize: 20}
                    });

                    // robin应该是：是否定时更新
                    if (chartOptions.robin) {
                        chartObject.loadData();

                        chartObject.interval = setInterval(function () {
                            chartObject.loadData();
                        }, chartOptions.timeSpan);
                    } else {
                        chartObject.loadData();
                    }
                },

                loadData: function () {
                    var chartObject = this;
                    var chartOptions = chartObject.options;

                    //非ajax直接显示数据的情况
                    if (!chartOptions.ajax) {
                        chartObject.formatOption();
                        chartObject.chart.setOption(chartOptions.EchartsOption, true);
                        chartObject.chart.hideLoading();
                        return;
                    }

                    var config = {};

                    //以下是ajax的情况下
                    if (chartOptions.getConfig) {
                        config = chartOptions.getConfig();
                    }
                    $.post(chartOptions.URL, config, function (response) {

                        //legend变短的情况，截断旧的legend，方式合并数据时留下脏数据
                        if (chartOptions.EchartsOption.legend && response.legend) {
                            if (chartOptions.EchartsOption.legend.length > response.legend.length) {
                                chartOptions.EchartsOption.legend = [];
                            }
                        }

                        //x轴
                        if (chartOptions.EchartsOption.xAxis && response.xAxis) {
                            if (chartOptions.EchartsOption.xAxis.length > response.xAxis.length) {
                                chartOptions.EchartsOption.xAxis = [];
                            }

                            //同时还得分析series中各data元素变短的情况
                            for (i in chartOptions.EchartsOption.xAxis) {
                                if (chartOptions.EchartsOption.xAxis[i].data && response.xAxis[i].data) {
                                    if (chartOptions.EchartsOption.xAxis[i].data.length > response.xAxis[i].data.length) {
                                        chartOptions.EchartsOption.xAxis[i].data = [];
                                    }
                                }
                            }
                        }

                        //series变短的情况
                        if (chartOptions.EchartsOption.series && response.series) {
                            if (chartOptions.EchartsOption.series.length > response.series.length) {
                                chartOptions.EchartsOption.series = [];
                            }

                            //同时还得分析series中各data元素变短的情况
                            for (i in chartOptions.EchartsOption.series) {
                                if (chartOptions.EchartsOption.series[i].data && response.series[i].data) {
                                    if (chartOptions.EchartsOption.series[i].data.length > response.series[i].data.length) {
                                        chartOptions.EchartsOption.series[i].data = [];
                                    }
                                }
                            }
                        }

                        //放心大胆地合并了
                        chartOptions.EchartsOption = $.extend(true, {}, chartOptions.EchartsOption, response);

                        chartObject.formatOption();
                        chartObject.chart.setOption(chartOptions.EchartsOption, true);
                        chartObject.chart.hideLoading();
                    }, 'json');
                },

                formatOption: function () {
                    var chartObject = this;
                    var chartOptions = chartObject.options;

                    if (chartOptions.formatNumber) {
                        chartObject.formatNumber(chartOptions.EchartsOption.legend);

                        for (i in chartOptions.EchartsOption.series) {
                            chartObject.formatNumber(chartOptions.EchartsOption.series[i].data);
                        }
                    }
                },

                //对数组中的各元素进行格式化
                formatNumber: function (dataArray) {
                    for (i in dataArray) {
                        if (typeof(dataArray[i]) == 'number') {
                            dataArray[i] = $.fn.GeneralChart.numberFormat(dataArray[i]);
                        } else if (typeof(dataArray[i] == 'object')) {
                            for (ii in dataArray[i]) {
                                if (typeof(dataArray[i][ii]) == 'number') {
                                    dataArray[i][ii] = $.fn.GeneralChart.numberFormat(dataArray[i][ii]);
                                }
                            }
                        }
                    }
                }
            };

            return chartObject;
        },
        createID: function () {
            return 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
                var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
                return v.toString(16);
            });
        },
        numberFormat: function (number) {
            return number.toFixed(2) - 0;
        }
    };
})(jQuery);

//默认属性
(function ($) {
    $.fn.GeneralChart.defaultOptions = {
        ajax: false, //是否ajax获取数据
        URL: '',    //ajax的URL
        robin: false, //是否动态刷新
        timeSpan: 5000, //刷新时间间隔
        getConfig: null, //获取config的函数
        formatNumber: true,//对数字进行格式化，防止太多的小数
        EchartsOption: null, //ECharts的option结构，当图表的样式或整个图表都在前端生成时使用
        chartContainer: 'ChartContainer'
    };
})(jQuery);
