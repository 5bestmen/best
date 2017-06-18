/**
 * Created by 洪祥 on 15/7/9.
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

    $.fn.RobinTable = {
        //初始化方法
        init: function (options) {
            for (var i = 0; i < options.columns.length; i++) {
                //初始化列参数
                options.columns[i] = $.extend({}, $.fn.RobinTable.defaultOptions.column, options.columns[i]);
            }
            //初始化标题参数
            options.captain = $.extend({}, $.fn.RobinTable.defaultOptions.captain, options.captain);

            //初始化表格参数
            options = $.extend({}, $.fn.RobinTable.defaultOptions.table, options);

            //如果没有定义编号则设置默认GUID编号
            if (!options.id) {
                options.id = $.fn.RobinTable.createID();
            }

            var robinTableObject = {
                options: options,
                table: null,
                tbody: null,
                rowNumber: 0,

                buildTableHeader: function () {
                    var tableObject = this;
                    var tableOptions = tableObject.options;

                    //创建表格元素
                    var table = $("<table id='{id}' class='{clazz}'></table>"
                        .format({id: tableOptions.id, clazz: tableOptions.tableClass}));
                    table.appendTo($("#" + tableOptions.TableContainer));

                    tableObject.table = table;

                    //创建标题
                    if (options.captain.show) {
                        $("<caption class='{clazz}'>{text}</caption>"
                            .format({
                                clazz: tableOptions.captain.captainClass,
                                text: tableOptions.captain.text
                            })).appendTo(table);
                    }

                    //创建表头
                    var thead = $("<thead></thead>");
                    thead.appendTo(table);
                    var tr = $("<tr></tr>");
                    tr.appendTo(thead);

                    for (var i in tableOptions.columns) {
                        var th = $("<th class='{clazz}'>{title}</th>".format({
                            clazz: tableOptions.columns[i].headerClass,
                            title: tableOptions.columns[i].title
                        }));
                        th.appendTo(tr);
                    }

                    var tbody = $("<tbody></tbody>");
                    tbody.appendTo(table);
                    tableObject.tbody = tbody;
                },

                load: function () {
                    var tableObject = this;
                    var tableOptions = tableObject.options;

                    if (tableOptions.robin) {
                        tableObject.loadData();

                        setInterval(function () {
                            tableObject.loadData();
                        }, tableOptions.timeSpan);
                    } else {
                        tableObject.loadData();
                    }
                },

                loadData: function () {
                    var tableObject = this;
                    var tableOptions = tableObject.options;
                    var tbody = tableObject.tbody;
                    var colOptions = tableObject.options.columns;

                    $.post(tableOptions.URL, tableOptions.config, function (response) {
                        //未填充数据
                        if (!tbody.children().length) {
                            for (i in response) {
                                var tr = $("<tr></tr>");
                                tr.appendTo(tbody);

                                for (var col in colOptions) {
                                    var cell = $("<td class='{clazz}'></td>".format({
                                        clazz: colOptions[col].columnClass,
                                    }));

                                    cell.appendTo(tr);
                                }
                            }
                        }

                        tableObject.fillTable(response);
                    }, 'json');
                },
                fillTable: function (response) {
                    var tableObject = this;
                    var tableOptions = tableObject.options;
                    var tbody = tableObject.tbody;
                    var colOptions = tableObject.options.columns;

                    var count = Math.min(response.length, tbody.children().length);
                    var rows = tbody.children();

                    for (var i = 0; i < count; i++) {
                        var tds = rows[i].children;
                        var data;

                        for (var col in colOptions) {
                            if (colOptions[col].type === 'number') {
                                data = $.fn.RobinTable.numberFormat(response[i][colOptions[col].id], colOptions[col].format);
                                tableOptions.numberFunction(tds[col], data, tableObject.table, response, i, col);
                            } else if (colOptions[col].type === 'string') {
                                data = response[i][colOptions[col].id];
                                tableOptions.stringFunction(tds[col], data, tableObject.table, response, i, col);
                            } else {
                                data = response[i][colOptions[col].id];

                                for (var o in data) {
                                    if (typeof data[o] == 'number') {
                                        data[o] = $.fn.RobinTable.numberFormat(data[o], colOptions[col].format);
                                    }
                                }
                                tableOptions.objectFunction(tds[col], data, tableObject.table, response, i, col);
                            }
                        }
                    }
                }
            };

            robinTableObject.buildTableHeader();
            return robinTableObject;
        },
        createID: function () {
            return 'xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
                var r = Math.random() * 16 | 0, v = c == 'x' ? r : (r & 0x3 | 0x8);
                return v.toString(16);
            });
        },
        numberFormat: function (number, pattern) {
            var negFlag = "false";
            var str = Number(number).toString();
            if (str.indexOf("-") == 0) {
                negFlag = "true";
                str = str.replace("-", "");
                number = -number;
            }
            var strInt;
            var strFloat;
            var formatInt;
            var formatFloat;
            if ($.trim(str) == "")
                return "";
            //判断模式串是否有小数格式
            if (/\./g.test(pattern)) {
                formatInt = pattern.split('.')[0];
                formatFloat = pattern.split('.')[1];
            } else {
                formatInt = pattern;
                formatFloat = null;
            }
            if (/\./g.test(str)) {
                //如果字符串有小数
                if (formatFloat != null) {
                    var tempFloat = Math.round(parseFloat('0.' + str.split('.')[1]) * Math.pow(10, formatFloat.length)) / Math.pow(10, formatFloat.length);
                    strInt = (Math.floor(number) + Math.floor(tempFloat)).toString();
                    strFloat = /\./g.test(tempFloat.toString()) ? tempFloat.toString().split('.')[1] : '0';
                } else {
                    strInt = Math.round(number).toString();
                    strFloat = '0';
                }
            } else {
                strInt = str;
                strFloat = '0';
            }
            //处理整数数位的格式化
            if (formatInt != null) {
                var outputInt = '';
                var zero = formatInt.match(/0*$/)[0].length;
                var comma = null;
                if (/,/g.test(formatInt)) {
                    comma = formatInt.match(/,[^,]*/)[0].length - 1;
                }
                var newReg = new RegExp('(\\d{' + comma + '})', 'g');
                if (strInt.length < zero) {
                    outputInt = new Array(zero + 1).join('0') + strInt;
                    outputInt = outputInt.substr(outputInt.length - zero, zero);
                } else {
                    outputInt = strInt;
                }
                outputInt = outputInt.substr(0, outputInt.length % comma) + outputInt.substring(outputInt.length % comma).replace(newReg, (comma != null ? ',' : '') + '$1');
                outputInt = outputInt.replace(/^,/, '');
                strInt = outputInt;
            }
            //处理小数位的格式化
            if (formatFloat != null) {
                var outputFloat = '';
                var zero = formatFloat.match(/^0*/)[0].length;
                if (strFloat.length < zero) {
                    outputFloat = strFloat + new Array(zero + 1).join('0');
                    var outputFloat1 = outputFloat.substring(0, zero);
                    var outputFloat2 = outputFloat.substring(zero, formatFloat.length);
                    outputFloat = outputFloat1 + outputFloat2.replace(/0*$/, '');
                } else {
                    //如果小数是0，而且模式串的小数格式中也不包含0，则只保留整数部分。
                    if (strFloat == 0 && zero == 0)
                        outputFloat = '';
                    else
                        outputFloat = strFloat.substring(0, formatFloat.length);
                }
                strFloat = outputFloat;
            } else {
                if (pattern != '' || (pattern == '' && strFloat == '0'))
                    strFloat = '';
            }
            if (negFlag == "true")
                return "-" + strInt + (strFloat == '' ? '' : '.' + strFloat);
            else
                return strInt + (strFloat == '' ? '' : '.' + strFloat);
        }


    };
})(jQuery);

//默认属性
(function ($) {
    $.fn.RobinTable.defaultOptions = {
        table: {
            URL: '',
            robin: false,
            timeSpan: 5000,
            config: {},
            tableClass: 'table table-bordered table-hover table-responsive',
            TableContainer: 'RTContainer',
            numberFunction: function (td, data, table, response, row, col) {
                td.innerText = data;
            },
            stringFunction: function (td, data, table, response, row, col) {
                td.innerText = data;
            },
            objectFunction: function (td, data, table, response, row, col) {
            }
        },
        captain: {
            show: false,
            text: 'captain',
            captainClass: ''
        },
        column: {
            id: 'field',
            title: 'field',
            type: 'string',
            format: null,
            columnClass: '',
            headerClass: ''
        }
    };
})(jQuery);
