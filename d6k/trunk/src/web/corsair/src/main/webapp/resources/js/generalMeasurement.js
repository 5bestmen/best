/**
 * Created by ChengXi on 2015/7/20.
 */
(function ($) {
    $.fn.GeneralMeasurement = {

        init: function () {

            var meaObject = {

                measurementData: null,
                measurementElements: null,

                normalMeasurementData : null,
                normalMeasurementElements : null,

                magicMeasurementData : null,
                magicMeasurementElements : null,

                calculateMeasurementData : null,
                calculateMeasurementElements : null,

                load: function () {

                    this.measurementData = [];

                    this.normalMeasurementData = [];
                    this.normalMeasurementElements = [];
                    this.magicMeasurementData = [];
                    this.magicMeasurementElements = [];
                    this.calculateMeasurementData = [];
                    this.calculateMeasurementElements = [];

                    this.measurementElements = $(".measurement");

                    for (var i = 0; i < this.measurementElements.length; i++) {
                        var measurement = {};
                        var measurementElement = this.measurementElements[i];

                        measurement.psrID = measurementElement.dataset.psrid;
                        var index = parseInt(measurementElement.dataset.templateindex);

                        if (isNaN(index)) {
                            index = -1;
                        }
                        measurement.templateIndex = index;
                        if (this.measurementElements[i].dataset.measurementid != null) {
                            measurement.measurementID = this.measurementElements[i].dataset.measurementid.replace(/(^\s*)|(\s*$)/g, '');
                        } else {
                            measurement.measurementID = "";
                        }
                        measurement.measurementType = measurementElement.dataset.measurementtype;
                        measurement.magic = measurementElement.dataset.magic;
                        measurement.calculate = measurementElement.dataset.calculate == 'true';
                        measurement.name = measurementElement.dataset.name;

                        if (measurement.magic) {
                            this.magicMeasurementData.push(measurement);
                            this.magicMeasurementElements.push(measurementElement);
                        } else if (measurement.calculate == true) {
                            this.calculateMeasurementData.push(measurement);
                            this.calculateMeasurementElements.push(measurementElement);
                        } else {
                            this.normalMeasurementData.push(measurement);
                            this.normalMeasurementElements.push(measurementElement);
                        }
                        //this.measurementData.push(measurement);
                    }

                    //meaObject.dataUpdate();
                    try {
                        meaObject.normalDataUpdate();
                    } catch (e) {
                        console.log(e);
                    }

                    try {
                        meaObject.magicDataUpdate();
                    } catch (e) {
                        console.log(e);
                    }

                    try {
                        meaObject.calculateDataUpdate();
                    } catch (e) {
                        console.log(e);
                    }


                    self.setInterval(function () {
                        try {
                            meaObject.normalDataUpdate();
                        } catch (e) {
                            console.log(e);
                        }

                        try {
                            meaObject.magicDataUpdate();
                        } catch (e) {
                            console.log(e);
                        }

                        try {
                            meaObject.calculateDataUpdate();
                        } catch (e) {
                            console.log(e);
                        }
                    }, 5000);
                },

                normalDataUpdate: function() {
                    console.log('normal');
                    var measurementData = this.normalMeasurementData;
                    var measurementElements = this.normalMeasurementElements;
                    var url = '/api/measurement/normal';

                    this.dataUpdate(measurementData, measurementElements, url);
                },

                magicDataUpdate: function() {
                    console.log('magic');
                    var measurementData = this.magicMeasurementData;
                    var measurementElements = this.magicMeasurementElements;
                    var url = '/api/measurement/magic';

                    this.dataUpdate(measurementData, measurementElements, url);
                },

                calculateDataUpdate: function() {
                    console.log('calc');
                    var measurementData = this.calculateMeasurementData;
                    var measurementElements = this.calculateMeasurementElements;
                    var url = '/api/measurement/calculate';

                    this.dataUpdate(measurementData, measurementElements, url);
                },

                dataUpdate: function (measurementData, measurementElements, url) {

                    if (measurementData.length == 0) {
                        return;
                    }
                    $.ajax({
                        type: 'POST',
                        dataType: 'json',
                        contentType: 'application/json',
                        url: url,
                        data: JSON.stringify(measurementData),
                        success: function (response, textStatus, jqXHR) {
                            for (var i in response) {

                                if (response[i] == null) {
                                    continue;
                                }

                                //遥测或电度时
                                if (measurementData[i].measurementType != 1) {
                                    //if (response[i].text && !(i >= 11 && (i-11)%4 === 0)) { // 如果text不为空则优先显示text
                                    if (response[i].text) { // 如果text不为空则优先显示text
                                        measurementElements[i].innerHTML = response[i].text;
                                    //} else if (!(i >= 11 && (i-11)%4 === 0)){
                                    } else {
                                        measurementElements[i].innerHTML = meaObject.numberFormat(response[i].data);
                                    }

                                    /*if(i >= 11 && (i-11)%4 === 0){
                                        if (response[i].data == 3) {
                                            $(measurementElements[i]).removeClass("red-bg").addClass('navy-bg');
                                        } else {
                                            $(measurementElements[i]).removeClass("navy-bg").addClass('red-bg');
                                        }
                                    }*/
                                } else {
                                    if (response[i].data == 0) {
                                        $(measurementElements[i]).removeClass("red-bg").addClass('navy-bg');
                                    } else {
                                        $(measurementElements[i]).removeClass("navy-bg").addClass('red-bg');
                                    }

                                    if (response[i].text) {
                                        $(measurementElements[i]).find('.YX')[0].innerText = response[i].text;
                                    }
                                }
                            }

                        }
                    });
                },

                numberFormat: function (number) {
                    if (typeof (number) == 'number') {
                        return number.toFixed(2) - 0;
                    } else {
                        return "NAN";
                    }
                }
            };

            return meaObject;
        }
    };

})(jQuery);