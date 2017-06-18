/**
 * Created by 洪祥 on 15/7/28.
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

    $.fn.MeanSelector = {
        //初始化方法
        init: function (divID, chartClass, psrID) {
            var selectorObject = {
                divID: null,
                chartClass: null,
                psrID: null,

                dataTypeDiv: null,
                chart:null,
                dateType: 'day',
                dayButtonID: $.fn.MeanSelector.createID(),
                monthButtonID: $.fn.MeanSelector.createID(),
                yearButtonID: $.fn.MeanSelector.createID(),
                accumulateButtonID: $.fn.MeanSelector.createID(),
                chartID: $.fn.MeanSelector.createID(),
                dateID: $.fn.MeanSelector.createID(),
                radioName: $.fn.MeanSelector.createID(),

                dayDate: {
                    elem: '',
                    format: 'YYYY/MM/DD hh:mm:ss',
                    min: '2000/01/01',
                    max: laydate.now(),
                    istime: true,
                    istoday: false
                },

                chartConfig:{
                    psrID: null,
                    dataType: null,
                    dateType: null,
                    date: null,
                    index: null,
                    magic:null,
                    chartName:null
                },

                buttonGroupTemplate: "<div class='row'>" +
                "<div class='col-md-4' style='float: right;'>" +
                "<div class='input-group'>" +
                /*"<span class='input-group-addon'>日期</span>" +*/
                "<input readonly class='laydate-icon layer-date' id='{dateID}' style='color:#000000;background-image: url(/resources/img/Calendar.png);padding-left: 10px;'>" +
                "</div>" +
                "</div>" +
                "<div class='col-md-4' style='float: right'>" +
                "<div class='btn-group btn-group-justified' role='group'>" +
                "<div class='btn-group' role='group'>" +
                "<button type='button' class='btn btn-default btn-success' data-date-type='day' id='{dayButtonID}'>日</button>" +
                "</div>" +
                "<div class='btn-group' role='group'>" +
                "<button type='button' class='btn btn-default' data-date-type='month' id='{monthButtonID}'>月</button>" +
                "</div>" +
                "<div class='btn-group' role='group'>" +
                "<button type='button' class='btn btn-default' data-date-type='year' id='{yearButtonID}'>年</button>" +
                "</div>" +
                "<div class='btn-group' role='group'>" +
                "<button type='button' class='btn btn-default' data-date-type='accumulate' id='{accumulateButtonID}'>累计</button>" +
                "</div>" +
                "</div>" +
                "</div>" +
                "</div>",
                dataTypeDivTemplate: "<div class='row'></div>",
                dataTypeTemplate: "<div class='col-lg-3 col-md-3'>" +
                "<label class='radio-inline'>" +
                "<input type='radio'" +
                "name='{radioName}'" +
                "data-type='{dataType}'" +
                "data-magic='{magic}'" +
                "data-chart='{chart}'" +
                "value='{radioValue}'>{radioText}" +
                "</label>" +
                "</div>",
                chartTemplate: "<div id='{chartID}' class='{chartClass}'></div>",

                load: function () {
                    var selectorObject = this;
                    selectorObject.createElements();
                    selectorObject.createChart();
                },
                createElements: function () {
                    var selectorObject = this;

                    $(selectorObject.buttonGroupTemplate.format({
                        dayButtonID: selectorObject.dayButtonID,
                        monthButtonID: selectorObject.monthButtonID,
                        yearButtonID: selectorObject.yearButtonID,
                        accumulateButtonID: selectorObject.accumulateButtonID,
                        dateID: selectorObject.dateID
                    })).appendTo("#" + selectorObject.divID);

                    selectorObject.dataTypeDiv = $(selectorObject.dataTypeDivTemplate);
                    selectorObject.dataTypeDiv.appendTo("#" + selectorObject.divID);

                    $(selectorObject.chartTemplate.format({
                        chartID: selectorObject.chartID,
                        chartClass: selectorObject.chartClass
                    })).appendTo("#" + selectorObject.divID);

                    $("#" + selectorObject.dayButtonID).bind("click", selectorObject.dayButtonID, selectorObject.onDateButtonClick);
                    $("#" + selectorObject.monthButtonID).bind("click", selectorObject.monthButtonID, selectorObject.onDateButtonClick);
                    $("#" + selectorObject.yearButtonID).bind("click", selectorObject.yearButtonID, selectorObject.onDateButtonClick);
                    $("#" + selectorObject.accumulateButtonID).bind("click", selectorObject.accumulateButtonID, selectorObject.onDateButtonClick);

                    laydate(selectorObject.dayDate);
                    $('#' + selectorObject.dateID)[0].value = laydate.now(0, 'YYYY/MM/DD hh:mm:ss');
                    selectorObject.getDataType(selectorObject.dateType);
                    selectorObject.dayDate.choose = selectorObject.onReDraw;
                },
                createChart: function () {
                    var option = {
                        ajax: true,
                        URL: '/api/charts/data',
                        robin: false,
                        timeSpan: 10000,
                        getConfig: function(){return selectorObject.chartConfig;},
                        formatNumber: true,
                        EchartsOption: {
                            grid: {
                                y:50,
                                x2:40,
                                y2:30
                            }
                        },
                        chartContainer: selectorObject.chartID
                    };

                    selectorObject.chart = $.fn.GeneralChart.init(option);
                },
                onDateButtonClick: function (event) {
                    $("#" + selectorObject.dayButtonID).removeClass("btn-success");
                    $("#" + selectorObject.yearButtonID).removeClass("btn-success");
                    $("#" + selectorObject.monthButtonID).removeClass("btn-success");
                    $("#" + selectorObject.accumulateButtonID).removeClass("btn-success");
                    $("#" + event.data).addClass("btn-success");
                    selectorObject.dateType = $("#" + event.data).data('date-type');

                    selectorObject.getDataType(selectorObject.dateType);
                },
                getDataType: function (dataType) {
                    $.post('/api/charts/type', {
                        psrID: selectorObject.psrID,
                        dateType: dataType
                    }, function (response) {
                        var container = selectorObject.dataTypeDiv[0];
                        if (container.childElementCount != 0) {
                            $(container).empty();
                        }

                        response.sort(function (a, b) {
                            return a.index - b.index;
                        });

                        for (var i in response) {
                            var check = $(selectorObject.dataTypeTemplate.format({
                                radioName: selectorObject.radioName,
                                radioValue: response[i].index,
                                radioText: response[i].name,
                                dataType:response[i].type,
                                magic:response[i].magic,
                                chart:response[i].chartName
                            }));

                            $(check[0].children[0].children[0]).bind('change', selectorObject.onReDraw);
                            check.appendTo(container);
                        }

                        if (container.childElementCount != 0) {
                            container.children[0].children[0].children[0].checked = true;
                            selectorObject.onReDraw();
                        }
                    }, 'json');
                },
                onReDraw: function () {
                    var date = $('#' + selectorObject.dateID)[0].value;
                    var index = null;
                    var dataType = null;
                    var magic = null;
                    var chartName = null;
                    var radios = $("input[name='{radioName}']".format({radioName: selectorObject.radioName}));

                    for (var r in radios) {
                        if (radios[r].checked === true) {
                            index = radios[r].value;
                            dataType = $(radios[r]).data('type');
                            magic = $(radios[r]).data('magic');
                            chartName = $(radios[r]).data('chart');
                            break;
                        }
                    }

                    selectorObject.chart.setRobin(selectorObject.dateType === 'day');

                    selectorObject.chartConfig.date = date;
                    selectorObject.chartConfig.dateType = selectorObject.dateType;
                    selectorObject.chartConfig.index = index;
                    selectorObject.chartConfig.psrID = selectorObject.psrID;
                    selectorObject.chartConfig.dataType = dataType;
                    selectorObject.chartConfig.magic = magic;
                    selectorObject.chartConfig.chartName = chartName;
                    selectorObject.chart.load();
                }
            };

            selectorObject.divID = divID;
            selectorObject.chartClass = chartClass;
            selectorObject.dayDate.elem = '#' + selectorObject.dateID;
            selectorObject.psrID = psrID;
            return selectorObject;
        },
        createID: function () {
            return 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
                var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
                return v.toString(16);
            });
        }
        ,
        numberFormat: function (number) {
            return number.toFixed(2) - 0;
        }
    }
    ;
})
(jQuery);