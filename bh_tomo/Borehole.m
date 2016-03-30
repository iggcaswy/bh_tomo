classdef Borehole < handle
    %BOREHOLE Class to hold borehole parameters
    
    properties
        name
        X
        Y
        Z
        Xmax
        Ymax
        Zmax
        Z_surf
        Z_water
        scont
        acont
        diam
        fdata
    end
    
    methods
        function obj = Borehole(n)
            obj.name = n;
            obj.X = 0;
            obj.Y = 0;
            obj.Z = 0;
            obj.Xmax = 0;
            obj.Ymax = 0;
            obj.Zmax = 0;
            obj.Z_surf = 0;
            obj.Z_water = NaN;
            obj.diam = 0;
            obj.scont = [];
            obj.acont = [];
            obj.fdata = [0 0 0;0 0 0];
        end
        function set.name(obj, n)
            if ischar(n)
                obj.name = n;
            else
                error('Borehole name must be a string')
            end
        end
        function set.X(obj,value)
            if isnumeric(value)
                obj.X = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Y(obj,value)
            if isnumeric(value)
                obj.Y = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Z(obj,value)
            if isnumeric(value)
                obj.Z = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Xmax(obj,value)
            if isnumeric(value)
                obj.Xmax = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Ymax(obj,value)
            if isnumeric(value)
                obj.Ymax = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Zmax(obj,value)
            if isnumeric(value)
                obj.Zmax = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Z_surf(obj,value)
            if isnumeric(value)
                obj.Z_surf = value;
            else
                error('Value must be numeric')
            end
        end
        function set.Z_water(obj,value)
            if isnumeric(value)
                obj.Z_water = value;
            else
                error('Value must be numeric')
            end
        end
        function set.diam(obj,value)
            if isnumeric(value)
                obj.diam = value;
            else
                error('Value must be numeric')
            end
        end
        function set.fdata(obj,value)
            if isnumeric(value)
                if size(value,2)~=3
                    error('Borehole data should be nData x 3')
                end
                obj.fdata = value;
            else
                error('Value must be numeric')
            end
        end
    end
    
    methods (Static)
        function [x,y,z,c] = project(fdata,depth,name)
            % function [x,y,z,c] = project(fdata,depth,name)
            %  Project measurements points on borehole trajectory
            %
            % fdata: matrix [n x 3]
            %          borehole trajectory (x,y,elev),
            %          sorted from top to bottom of borehole
            % depth: vector [1 x n2]
            %          depth of the n2 measurement points (from top to
            %          bottom)
            % name:  name of borehole
            %
            % x:     x coordinates of measurement points [1 x n2]
            % y:     y coordinates of measurement points [1 x n2]
            % z:     elevation of measurement points [1 x n2]
            % c:     direction cosines at meas .points [n2 x 3], pointing
            %           downwards
            %
            
            npts = length(depth);
            x = zeros(1,npts);
            y = zeros(1,npts);
            z = zeros(1,npts);
            c = zeros(npts,3);
            
            depthBH = [0; cumsum(sqrt(sum( (diff(fdata,1)).^2, 2)))];
            
            for n=1:npts
                % trajectory is cut into segments
                % start index of segment in which the point is
                i1 = find(depth(n)>=depthBH);
                if isempty(i1)
                    errordlg(['Measurement pt outside borehole: ',name])
                    x = zeros(1,npts);
                    y = zeros(1,npts);
                    z = zeros(1,npts);
                    c = zeros(npts,3);
                    return
                end
                i1 = i1(end);
                % end index of segment
                i2 = find(depth(n)<depthBH);
                if isempty(i2)
                    errordlg(['Measurement pt outside borehole: ',name])
                    x = zeros(1,npts);
                    y = zeros(1,npts);
                    z = zeros(1,npts);
                    c = zeros(npts,3);
                    return
                end
                i2 = i2(1);
                
                d = sqrt(sum((fdata(i2,1:3)-fdata(i1,1:3)).^2));
                % direction cosine of the segment
                l = (fdata(i2,1:3)-fdata(i1,1:3))./d;
                
                % distance between start of segment and measurement point
                d2 = depth(n) - depthBH(i1);
                
                x(n) = fdata(i1,1) + d2*l(1);
                y(n) = fdata(i1,2) + d2*l(2);
                z(n) = fdata(i1,3) + d2*l(3);
                c(n,:) = l;
            end
        end
    end
end
