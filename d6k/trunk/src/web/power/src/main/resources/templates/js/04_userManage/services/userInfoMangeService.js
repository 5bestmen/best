/**
 * Created by lojoso on 2017/1/17.
 */

DaQOCloud.factory('userInfoMangeService',function () {
    return {
        getUserList: function (adminid,pwd) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getUserInfoList?adminid="+adminid+"&adminpwd="+pwd,{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        deleteUser:function(adminid,pwd,targetid){
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/deleteUserInfo?adminid="+adminid+"&adminpwd="+pwd+"&targetid="+targetid,{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
            })
        },
        getUser: function (adminid,pwd,userid) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getUserInfo?adminid="+adminid+"&adminpwd="+pwd+"&userid="+userid,{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        getAllPjInfo: function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getAllPjInfo",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        getAllImei: function () {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/getAllImei",{method:'post'}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
                return res.json();
            }).then(function(result){
                return result;
            })
        },
        updateUser: function (obj) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/updateUser",{method:'post',body:angular.toJson(obj)}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
            })
        },
        AddUser: function (obj) {
            return fetch("http://172.16.42.37:8080/DaQOCloudSystem/InfoManage/insertUser",{method:'post',body:angular.toJson(obj)}).then(function(res) {
                console.log("Response succeeded?", JSON.stringify(res.ok));
            })
        }
    }
})