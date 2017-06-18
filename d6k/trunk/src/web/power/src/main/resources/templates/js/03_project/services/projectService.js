/**
 * Created by reed on 2017/1/6.
 */
DaQOCloud.factory('projectService',function ($http) {
    /*return {
        getPjInfoList : function (userId) {
            return $http.get('http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getProjectInfo?userid='+userId).then(function (data) {
                return data.data;
            },function (data, status) {

            })
        },

        getDeviceKind : function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getDeviceKind",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        },
        getUnitInfo : function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getUnitInfo",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            });
        }
    }*/
    return null;
})