/**
 * Created by reed on 2017/2/6.
 */
DaQOCloud.factory('macManageService',function () {
    return {
        getAllPjInfo:function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getAllPjInfo",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        getAllUnitInfo:function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getUnitInfo",{method:'post'}).then(function(res) {
                console.log("getAllUnitInfo succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        getDeviceKindInfo:function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getDeviceKind",{method:'post'}).then(function(res) {
                console.log("getDeviceKindInfo succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        getVendorInfo:function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getVendorInfo",{method:'get'}).then(function(res) {
                console.log("getVendorInfo succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        getDeviceModel:function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getDeviceModel",{method:'get'}).then(function(res) {
                console.log("getDeviceModel succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        getAllDeviceInfoList: function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getAllDeviceInfoList",{method:'get'}).then(function(res) {
                console.log("getAllDeviceInfoList succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        deleteDeviceInfo:function(targetid){
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/deleteDeviceInfo?targetid="+targetid,{method:'post'}).then(function(res) {
                console.log("deleteDeviceInfo succeeded?", JSON.stringify(res.ok));
            })
        },
        updataDeviceInfo:function (deviceInfo) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/updataDeviceInfo",{method:'post',body:angular.toJson(deviceInfo)}).then(function(res) {
                console.log("updataDeviceInfo succeeded?", JSON.stringify(res.ok));
            })
        },
        insertDeviceInfo:function (deviceInfo) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/insertDeviceInfo",{method:'post',body:angular.toJson(deviceInfo)}).then(function(res) {
                console.log("insertDeviceInfo succeeded?", JSON.stringify(res.ok));
            })
        }
    }
})