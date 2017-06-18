/**
 * Created by 洪祥 on 15/7/29.
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

    $.fn.PSREvent = {
        //初始化方法
        init: function (eventTableID, psrID) {
            var psrEvent = {
                eventContainerID: null,
                psrID: null,
                startTime: null,
                endTime: null,
                grid: null,
                startTimeID: $.fn.PSREvent.createID(),
                endTimeID: $.fn.PSREvent.createID(),
                searchButtonID: $.fn.PSREvent.createID(),
                eventDivID: $.fn.PSREvent.createID(),
                eventToolBarDivID: $.fn.PSREvent.createID(),

                gridOption: {
                    lang: 'zh-cn',
                    ajaxLoad: true,
                    loadAll: true,
                    loadURL: null,
                    columns: [
                        {
                            id: 'eventTime',
                            title: '事项时间',
                            type: 'string',
                            /*columnStyle:dataNo%2==1?'background:#fff':'background:#82accb;',*/
                            columnClass: 'text-left',
                            headerStyle:'background:#b4c8d8;color:black;',
                            resolution:function(value, record, column, grid, dataNo, columnNo){
                                var content = '';
                                if(dataNo%2==1){
                                    content += '<span class="even">'+value+'</span>';
                                }else{
                                    content += '<span class="odd">'+value+'</span>';
                                }
                                return content;
                            }

                        },
                        {
                            id: 'event',
                            title: '事项描述',
                            type: 'string',
                            columnClass: 'text-left',
                            headerStyle:'background:#b4c8d8;color:black;',
                            resolution:function(value, record, column, grid, dataNo, columnNo){
                                var content = '';
                                if(dataNo%2==1){
                                    content += '<span class="even">'+value+'</span>';
                                }else{
                                    content += '<span class="odd">'+value+'</span>';
                                }
                                return content;
                            }
                        },
                        {
                            id: 'type',
                            title: '事件类型',
                            type: 'string',
                            columnClass: 'text-left',
                            headerStyle:'background:#b4c8d8;color:black;',
                            resolution:function(value, record, column, grid, dataNo, columnNo){
                                var content = '';
                                if(dataNo%2==1){
                                    content += '<span class="even">'+value+'</span>';
                                }else{
                                    content += '<span class="odd">'+value+'</span>';
                                }
                                return content;
                            }
                        }
                    ],
                    gridContainer: null,
                    toolbarContainer: null,
                    tools: '',
                    pageSize: 10,
                    pageSizeLimit: [10, 20, 50],
                    onGridComplete : function(grid){
                        $(".even").parent().css("background","#82accb");
                        $(".odd").parent().css("background","rgb(31, 86, 134)");
                        $(".odd").parent().css("color","#fff");
                    }
                },
                template: "<div class='row'>" +
                "<div class='col-md-3'>" +
                "<div class='input-group'>" +
                "<span class='input-group-addon'>起始时间</span>" +
                "<input readonly='' class='laydate-icon layer-date' id='{startTimeID}' style='color:#000000;background-image: url(/resources/img/Calendar.png);padding-left: 10px;'>" +
                "</div>" +
                "</div>" +
                "<div class='col-md-3'>" +
                "<div class='input-group'>" +
                "<span class='input-group-addon'>结束时间</span>" +
                "<input readonly='' class='laydate-icon layer-date' id='{endTimeID}' style='color:#000000;background-image: url(/resources/img/Calendar.png);padding-left: 10px;'>" +
                "</div>" +
                "</div>" +
                "<div class='col-md-2'>" +
                "<button type='button' class='btn btn-success btn-block' id='{searchButtonID}'>" +
                "<i class='fa fa-search'></i>查询" +
                "</button>" +
                "</div>" +
                "</div>" +
                "<br>" +
                "<div id='{eventDivID}'>" +
                "</div>" +
                "<div class='dt-grid-toolbar-container' id='{eventToolBarDivID}'>" +
                "</div>",
                startTimeFormat: {
                    elem: null,
                    format: 'YYYY/MM/DD hh:mm:ss',
                    min: '2000-01-01 00:00:00', //设定最小日期为当前日期
                    max: laydate.now(), //最大日期
                    istime: true,
                    istoday: true
                },
                endTimeFormat: {
                    elem: null,
                    format: 'YYYY/MM/DD hh:mm:ss',
                    min: '2000-01-01 00:00:00',
                    max: laydate.now(),
                    istime: true,
                    istoday: true
                },

                load: function () {
                    this.createElement();
                    this.showTable();

                },

                createElement: function () {
                    //创建日期、按钮和其他框架
                    $(this.template.format({
                        startTimeID: this.startTimeID,
                        endTimeID: this.endTimeID,
                        searchButtonID: this.searchButtonID,
                        eventDivID: this.eventDivID,
                        eventToolBarDivID: this.eventToolBarDivID
                    })).appendTo("#" + this.eventContainerID);

                    //创建表格
                    this.gridOption.gridContainer = this.eventDivID;
                    this.gridOption.toolbarContainer = this.eventToolBarDivID;
                    this.grid = $.fn.DtGrid.init(this.gridOption);

                    this.startTimeFormat.elem = '#' + this.startTimeID;
                    this.endTimeFormat.elem = '#' + this.endTimeID;

                    this.startTimeFormat.choose = this.onStartTime;
                    this.endTimeFormat.choose = this.onEndTime;

                    laydate(this.startTimeFormat);
                    laydate(this.endTimeFormat);

                    $('#' + this.searchButtonID).bind("click", this.onSearchButtonClick);
                },
                onStartTime: function (datas) {
                    psrEvent.endTimeFormat.min = datas; //开始日选好后，重置结束日的最小日期
                    psrEvent.endTimeFormat.start = datas; //将结束日的初始值设定为开始日
                },
                onEndTime: function (datas) {
                    psrEvent.startTimeFormat = datas; //结束日选好后，重置开始日的最大日期
                },
                onSearchButtonClick: function () {
                    var timeItem = {
                        startTime: $('#' + psrEvent.startTimeID)[0].value,
                        endTime: $('#' + psrEvent.endTimeID)[0].value
                    };

                    sessionStorage.setItem(psrEvent.psrID, JSON.stringify(timeItem));
                    window.location.reload();
                },
                showTable: function () {
                    var startTime, endTime;
                    var timeItem = sessionStorage.getItem(this.psrID);

                    if (timeItem == null) {
                        startTime = laydate.now(-30, 'YYYY/MM/DD hh:mm:ss');
                        endTime = laydate.now(0, 'YYYY/MM/DD hh:mm:ss');
                    } else{
                        var data = JSON.parse(timeItem);
                        startTime = data.startTime;
                        endTime = data.endTime;
                    }

                    $('#' + this.startTimeID)[0].value = startTime;
                    $('#' + this.endTimeID)[0].value = endTime;

                    this.gridOption.loadURL = '/api/deviceEvent?from=' + startTime + '&to=' + endTime + '&device=' + this.psrID;

                    var table = $.fn.DtGrid.init(this.gridOption);
                    table.load();
                }

            };

            psrEvent.eventContainerID = eventTableID;
            psrEvent.psrID = psrID;

            return psrEvent;
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

//默认属性
(function ($) {
    $.fn.PSREvent.defaultOptions = {
        lang: 'zh-cn',
        ajaxLoad: true,
        loadAll: true,
        loadURL: '/api/deviceEvent',
        columns: [
            {
                id: 'eventTime',
                title: '事项时间',
                type: 'string',
                columnClass: 'text-left'
            },
            {
                id: 'event',
                title: '事项描述',
                type: 'string',
                columnClass: 'text-left'
            },
            {
                id: 'type',
                title: '事件类型',
                type: 'string',
                columnClass: 'text-left'
            }
        ],
        gridContainer: 'alarmTable',
        toolbarContainer: 'alarmToolBar',
        tools: '',
        pageSize: 10,
        pageSizeLimit: [10, 20, 50],
        onGridComplete : function(grid){
            $(".table-bordered>thead>tr>th").css("border","0px solid #ddd");
            $(".dt-grid th").css("text-align","#left");
            $(".even").parent().css("background","#82accb");
            $(".odd").parent().css("background","rgb(31, 86, 134)");
            $(".odd").parent().css("color","#fff");
        }
    };
})(jQuery);