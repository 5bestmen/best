/**
 * Created by lojoso on 2017/2/4.
 */
DaQOCloud.factory('unitManageService',function () {
    return {
        getUnitInfo : function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getUnitInfo",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        getProjectList:function(){
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getAllPjInfo",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        undateUnit:function (obj) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/updateUnit",{method:'post',body:angular.toJson(obj)}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
            })
        },
        addUnit:function (obj) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/addUnitInfo",{method:'post',body:angular.toJson(obj)}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
            })
        },
        deleteUnit:function (unitid) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/deleteUnitInfo?unitid="+unitid,{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
            })
        }
    }
})