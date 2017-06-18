/**
 * Created by reed on 2017/2/4.
 */

DaQOCloud.factory('proManageService',function () {
    return {
        getAllProInfoList: function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getAllProInfoList",{method:'get'}).then(function(res) {
                console.log("getProInfoList succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        deleteProInfo:function(targetid){
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/deleteProInfo?targetid="+targetid,{method:'post'}).then(function(res) {
                console.log("deleteProInfo succeeded?", JSON.stringify(res.ok));
            })
        },
        updataProInfo:function (pjInfo) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/updataProInfo",{method:'post',body:angular.toJson(pjInfo)}).then(function(res) {
                console.log("updataProInfo succeeded?", JSON.stringify(res.ok));
            })
        },
        insertProInfo:function (pjInfo) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/insertProInfo",{method:'post',body:angular.toJson(pjInfo)}).then(function(res) {
                console.log("insertProInfo succeeded?", JSON.stringify(res.ok));
            })
        }
    }
})