/**
 * Created by zyj on 2017/2/7.
 */

function BuildMenu() {
    var treeData;
    $.ajax({
        url: "ashx/MainMenuHandler.ashx",
        success: function (ret) {
            treedata = eval("(" + ret + ")");

            $("#Menu").tree({ data: treedata, onClick: function (node) {
                if (node.url != "") {
                    // OpenTab(node.text, node.url);   打开菜单
                }
                else if (node.motion != "") {
                    eval(node.motion);
                }
            }
            });

        },
        error: function (err) {
            alert(err);
        }
    });
}