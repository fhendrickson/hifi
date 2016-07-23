//
//  stats.qml
//  examples/utilities/render
//
//  Created by Zach Pomerantz on 2/8/2016
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or https://www.apache.org/licenses/LICENSE-2.0.html
//
import QtQuick 2.5
import QtQuick.Controls 1.4
import "../lib/plotperf"

Item {
    id: statsUI
    anchors.fill:parent

    Column {
        id: stats
        spacing: 8
        anchors.fill:parent
 
        property var config: Render.getConfig("Stats")
 
        function evalEvenHeight() {
            // Why do we have to do that manually ? cannot seem to find a qml / anchor / layout mode that does that ?
            return (height - spacing * (children.length - 1)) / children.length
        }

      
        PlotPerf {
           title: "Timing"
           height: parent.evalEvenHeight()
           object: parent.drawOpaqueConfig
           valueUnit: "ms"
           valueScale: 1
           valueNumDigits: "4"
           plots: [
            {
                   object: Render.getConfig("OpaqueRangeTimer"),
                   prop: "gpuTime",
                   label: "Opaque",
                   color: "#FFFFFF"
               }, 
               {
                   object: Render.getConfig("LinearDepth"),
                   prop: "gpuTime",
                   label: "LinearDepth",
                   color: "#00FF00"
               },{
                   object: Render.getConfig("SurfaceGeometry"),
                   prop: "gpuTime",
                   label: "SurfaceGeometry",
                   color: "#00FFFF"
               },
               {
                   object: Render.getConfig("RenderDeferred"),
                   prop: "gpuTime",
                   label: "DeferredLighting",
                   color: "#FF00FF"
               }
               ,
               {
                   object: Render.getConfig("ToneAndPostRangeTimer"),
                   prop: "gpuTime",
                   label: "tone and post",
                   color: "#FF0000"
               }
           ]
        }
    }

}

