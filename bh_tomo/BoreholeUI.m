classdef BoreholeUI < handle
    %BOREHOLEUI User interface to manage Boreholes
    
    properties
        boreholes
    end
    properties (Dependent)
        Position
        FontSize
    end
    properties (Access=private)
        handles
    end
    events
        coordinatesChanged, boreholeAdded, boreholeDeleted, boreholeEdited
    end
    methods
        function obj = BoreholeUI(varargin)
            obj.boreholes = Borehole.empty;
            obj.handles.hp = uipanel(varargin{:},...
            'Title','Boreholes',...
            'Visible','off',...
            'SizeChangedFcn', @obj.resizeUI);
            
            obj.addComponents();
            obj.handles.hp.Visible = 'on';
        end
        function set.boreholes(obj, b)
            if isa(b, 'Borehole')
                obj.boreholes = b;
            else
                error('Boreholes should be objects of class Borehole')
            end
            obj.updateList(1)
            obj.updateEdits()
        end
        function set.Position(obj, p)
            obj.handles.hp.Position = p;
        end
        function set.FontSize(obj, s)
            obj.handles.hp.FontSize = s+1;
            obj.handles.addBH.FontSize = s;
            obj.handles.removeBH.FontSize = s;
            obj.handles.importBH.FontSize = s;
            obj.handles.plotBH.FontSize = s;
            obj.handles.listBH.FontSize = s;
            obj.handles.textCoord.FontSize = s;
            obj.handles.textX.FontSize = s;
            obj.handles.textY.FontSize = s;
            obj.handles.textZ.FontSize = s;
            obj.handles.textCollar.FontSize = s;
            obj.handles.textBottom.FontSize = s;
            obj.handles.topX.FontSize = s;
            obj.handles.topY.FontSize = s;
            obj.handles.topZ.FontSize = s;
            obj.handles.bottomX.FontSize = s;
            obj.handles.bottomY.FontSize = s;
            obj.handles.bottomZ.FontSize = s;
            obj.handles.textZwater.FontSize = s;
            obj.handles.Zwater.FontSize = s;
            obj.handles.textZsurf.FontSize = s;
            obj.handles.Zsurf.FontSize = s;
            obj.handles.textDiam.FontSize = s;
            obj.handles.diameter.FontSize = s;
            obj.handles.contS.FontSize = s;
            obj.handles.contA.FontSize = s;
        end
    end
    
    methods (Access=private)
        function addComponents(obj)
            
            obj.handles.addBH = uicontrol('Style','pushbutton',...
                'String','Add',...
                'Units','points',...
                'Callback',@obj.addBH,...
                'Parent',obj.handles.hp);
            obj.handles.removeBH = uicontrol('Style','pushbutton',...
                'String','Remove',...
                'Units','points',...
                'Callback',@obj.removeBH,...
                'Parent',obj.handles.hp);
            obj.handles.importBH = uicontrol('Style','pushbutton',...
                'String','Import',...
                'Units','points',...
                'Callback',@obj.importBH,...
                'Parent',obj.handles.hp);
            obj.handles.plotBH = uicontrol('Style','pushbutton',...
                'String','Plot',...
                'Units','points',...
                'Callback',@obj.plotBH,...
                'Parent',obj.handles.hp);

            obj.handles.listBH = uicontrol('Style','listbox',...
                'Max',1,'Min',0,...
                'Units','points',...
                'Callback',@obj.listBH,...
                'Parent',obj.handles.hp);
            
            obj.handles.textCoord = uicontrol('Style','text',...
                'String','Coordinates',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.textX = uicontrol('Style','text',...
                'String','X:',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.textY = uicontrol('Style','text',...
                'String','Y:',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.textZ = uicontrol('Style','text',...
                'String','Elevation:',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.textCollar = uicontrol('Style','text',...
                'String','Collar',...
                'Units','points',...
                'HorizontalAlignment','center',...
                'Parent',obj.handles.hp);
            obj.handles.textBottom = uicontrol('Style','text',...
                'String','Bottom',...
                'Units','points',...
                'HorizontalAlignment','center',...
                'Parent',obj.handles.hp);
            
            obj.handles.topX = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.topX,...
                'Parent',obj.handles.hp);
            obj.handles.topY = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.topY,...
                'Parent',obj.handles.hp);
            obj.handles.topZ = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.topZ,...
                'Parent',obj.handles.hp);
            obj.handles.bottomX = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.bottomX,...
                'Parent',obj.handles.hp);
            obj.handles.bottomY = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.bottomY,...
                'Parent',obj.handles.hp);
            obj.handles.bottomZ = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.bottomZ,...
                'Parent',obj.handles.hp);

            obj.handles.textZsurf = uicontrol('Style','text',...
                'String','Elevation at surface:',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.Zsurf = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.Zsurf,...
                'Parent',obj.handles.hp);

            obj.handles.textZwater = uicontrol('Style','text',...
                'String','Elev. water:',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.Zwater = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.Zwater,...
                'Parent',obj.handles.hp);

            obj.handles.textDiam = uicontrol('Style','text',...
                'String','Diameter:',...
                'Units','points',...
                'HorizontalAlignment','right',...
                'Parent',obj.handles.hp);
            obj.handles.diameter = uicontrol('Style','edit',...
                'Units','points',...
                'Callback',@obj.diameter,...
                'Parent',obj.handles.hp);

            obj.handles.contS = uicontrol('Style','pushbutton',...
                'String','Constraints Veloc.',...
                'Units','points',...
                'Callback',@obj.contS,...
                'Parent',obj.handles.hp);
            obj.handles.contA = uicontrol('Style','pushbutton',...
                'String','Constraints Atten.',...
                'Units','points',...
                'Callback',@obj.contA,...
                'Parent',obj.handles.hp);

        end
        function resizeUI(obj,varargin)
            if isempty(obj.handles.hp)
                return
            end
            obj.handles.hp.Visible = 'off';
            oldUnits = obj.handles.hp.Units;
            obj.handles.hp.Units = 'points';

            p = obj.handles.hp.Position;
            width = p(3);  % prefered: 300
            height = p(4); % prefered: 360

            hSize = width/5;
            hSpace = width/60;
            hBorder = width/15;
            
            vFac = 0.8*height/360;
            if vFac<1
                vFac = 1;
            end
            if ispc
                vFac = 0.81*vFac;
            end
            vSize = 22*vFac;
            vSpace = 5*vFac;
            vBorderTop = 45*vFac;
            vBorder = 15*vFac;
            
            vSizeList = height-vBorderTop-2*vSpace-(vBorder+7*vSize+7*vSpace);
            
            obj.handles.addBH.Position = [hBorder height-vBorderTop hSize vSize];
            obj.handles.removeBH.Position = [hBorder+hSize+hSpace height-vBorderTop hSize vSize];
            obj.handles.importBH.Position = [hBorder+2*hSize+2*hSpace height-vBorderTop hSize vSize];
            obj.handles.plotBH.Position = [hBorder+3*hSize+3*hSpace height-vBorderTop hSize vSize];
            
            obj.handles.listBH.Position = [1.5*hBorder vBorder+7*vSize+7*vSpace width-3*hBorder vSizeList];
            obj.handles.textCoord.Position = [width/2-3/2*hSize-hSpace vBorder+6*vSize+6*vSpace hSize vSize];
            obj.handles.textX.Position = [width/2-3/2*hSize-hSpace vBorder+5*vSize+5*vSpace hSize vSize];
            obj.handles.textY.Position = [width/2-3/2*hSize-hSpace vBorder+4*vSize+4*vSpace hSize vSize];
            obj.handles.textZ.Position = [width/2-3/2*hSize-hSpace vBorder+3*vSize+3*vSpace hSize vSize];
            obj.handles.textCollar.Position = [width/2-hSize/2 vBorder+6*vSize+6*vSpace hSize vSize];
            obj.handles.textBottom.Position = [width/2+hSize/2+hSpace vBorder+6*vSize+6*vSpace hSize vSize];
            obj.handles.topX.Position = [width/2-hSize/2 vBorder+5*vSize+5*vSpace hSize vSize];
            obj.handles.topY.Position = [width/2-hSize/2 vBorder+4*vSize+4*vSpace hSize vSize];
            obj.handles.topZ.Position = [width/2-hSize/2 vBorder+3*vSize+3*vSpace hSize vSize];
            obj.handles.bottomX.Position = [width/2+hSize/2+hSpace vBorder+5*vSize+5*vSpace hSize vSize];
            obj.handles.bottomY.Position = [width/2+hSize/2+hSpace vBorder+4*vSize+4*vSpace hSize vSize];
            obj.handles.bottomZ.Position = [width/2+hSize/2+hSpace vBorder+3*vSize+3*vSpace hSize vSize];
            
            obj.handles.textZsurf.Position = [width/2-5/3*hSize-hSpace vBorder+2*vSize+2*vSpace 5/3*hSize vSize];
            obj.handles.Zsurf.Position = [width/2 vBorder+2*vSize+2*vSpace hSize vSize];
            
            obj.handles.textZwater.Position = [hBorder vBorder+vSize+vSpace hSize vSize];
            obj.handles.Zwater.Position = [hBorder+hSize+hSpace vBorder+vSize+vSpace hSize vSize];
            obj.handles.textDiam.Position = [hBorder+2*hSize+2*hSpace vBorder+vSize+vSpace 5/6*hSize vSize];
            obj.handles.diameter.Position = [hBorder+17/6*hSize+3*hSpace vBorder+vSize+vSpace hSize vSize];
            
            obj.handles.contS.Position = [width/2-2*hSize-hSpace-2 vBorder 2*hSize vSize];
            obj.handles.contA.Position = [width/2+hSpace vBorder 2*hSize vSize];
            
            obj.handles.hp.Visible = 'on';
            obj.handles.hp.Units = oldUnits;
        end
        
        function addBH(obj,varargin)
            name = myinputdlg('Borehole name');
            if isempty(name)
                return;
            end
            if isempty(char(name))
                return;
            end
            if isempty(obj.boreholes)
                obj.boreholes = Borehole(char(name));
            else
                obj.boreholes(end+1) = Borehole(char(name));
            end
            obj.updateList()
            obj.handles.listBH.Value = length(obj.boreholes);
            obj.updateEdits()
            obj.notify('boreholeAdded')
        end
        function removeBH(obj, varargin)
            no = obj.handles.listBH.Value;
            ind=1:length(obj.boreholes);
            ind = ind~=no;

            obj.boreholes = obj.boreholes(ind);

            obj.updateList()
            obj.updateEdits()
            obj.notify('boreholeDeleted')
        end
        function importBH(obj, varargin)
            [filename, pathname] = uigetfile('*.xyz', 'Import borehole');
            if isequal(filename,0) || isequal(pathname,0)
                return
            else
                name = filename(1:end-4);
                fdata = load([pathname,filename]);
            end
            if isempty(obj.boreholes)
                obj.boreholes = Borehole(char(name));
            else
                obj.boreholes(end+1) = Borehole(char(name));
            end
            obj.boreholes(end).X = fdata(1,1);
            obj.boreholes(end).Y = fdata(1,2);
            obj.boreholes(end).Z = fdata(1,3);
            obj.boreholes(end).Xmax = fdata(end,1);
            obj.boreholes(end).Ymax = fdata(end,2);
            obj.boreholes(end).Zmax = fdata(end,3);
            obj.boreholes(end).fdata = fdata;

            obj.updateList()
            obj.handles.listBH.Value = length(obj.boreholes);
            obj.updateEdits()
            obj.notify('boreholeAdded')
        end
        function plotBH(obj, varargin)
            if isempty(obj.boreholes), return, end

            figure
            for b=obj.boreholes
                plot3(b.fdata(:,1),...
                    b.fdata(:,2),...
                    b.fdata(:,3),'g','LineWidth',2)
                hold on
                plot3(b.X, b.Y, b.Z_surf,'ro')
                text(b.X, b.Y, ...
                    b.Z_surf, b.name)
            end
            grid on
            hold off
            xlabel('X [m]')
            ylabel('Y [m]')
            zlabel('Elevation [m]')
            set(gca,'DataAspectRatio',[1 1 1])

        end
        function listBH(obj,varargin)
            obj.updateEdits()
        end
        function topX(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                if obj.boreholes(no).X==obj.boreholes(no).fdata(1,1)
                    obj.boreholes(no).X = str2double( obj.handles.topX.String );
                    obj.boreholes(no).fdata(1,1) = obj.boreholes(no).X;
                else
                    % prepend fdata with new value
                    obj.boreholes(no).X = str2double( obj.handles.topX.String );
                    obj.boreholes(no).fdata = [obj.boreholes(no).X obj.boreholes(no).Y obj.boreholes(no).Z;
                        obj.boreholes(no).fdata];
                end
                obj.notify('coordinatesChanged');
            end
        end
        function topY(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                if obj.boreholes(no).Y==obj.boreholes(no).fdata(1,2)
                    obj.boreholes(no).Y = str2double( obj.handles.topY.String );
                    obj.boreholes(no).fdata(1,2) = obj.boreholes(no).Y;
                else
                    % prepend fdata with new value
                    obj.boreholes(no).Y = str2double( obj.handles.topY.String );
                    obj.boreholes(no).fdata = [obj.boreholes(no).X obj.boreholes(no).Y obj.boreholes(no).Z;
                        obj.boreholes(no).fdata];
                end
                obj.notify('coordinatesChanged');
            end
        end
        function topZ(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                if obj.boreholes(no).Z==obj.boreholes(no).fdata(1,3)
                    obj.boreholes(no).Z = str2double( obj.handles.topZ.String );
                    obj.boreholes(no).fdata(1,3) = obj.boreholes(no).Z;
                else
                    % prepend fdata with new value
                    obj.boreholes(no).Z = str2double( obj.handles.topZ.String );
                    obj.boreholes(no).fdata = [obj.boreholes(no).X obj.boreholes(no).Y obj.boreholes(no).Z;
                        obj.boreholes(no).fdata];                    
                end
                obj.notify('coordinatesChanged');
            end

        end
        function bottomX(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                if obj.boreholes(no).Xmax == obj.boreholes(no).fdata(end,1)
                    obj.boreholes(no).Xmax = str2double( obj.handles.bottomX.String );
                    obj.boreholes(no).fdata(end,1) = obj.boreholes(no).Xmax;
                else
                    % append new value to fdata
                    obj.boreholes(no).Xmax = str2double( obj.handles.bottomX.String );
                    obj.boreholes(no).fdata = [obj.boreholes(no).fdata;
                        obj.boreholes(no).X obj.boreholes(no).Y obj.boreholes(no).Z];
                end
                obj.notify('coordinatesChanged');
            end
        end
        function bottomY(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                if obj.boreholes(no).Ymax == obj.boreholes(no).fdata(end,2)
                    obj.boreholes(no).Ymax = str2double( obj.handles.bottomY.String );
                    obj.boreholes(no).fdata(end,2) = obj.boreholes(no).Ymax;
                else
                    % append new value to fdata
                    obj.boreholes(no).Ymax = str2double( obj.handles.bottomY.String );
                    obj.boreholes(no).fdata = [obj.boreholes(no).fdata;
                        obj.boreholes(no).X obj.boreholes(no).Y obj.boreholes(no).Z];
                end
                obj.notify('coordinatesChanged');
            end
        end
        function bottomZ(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                if obj.boreholes(no).Zmax == obj.boreholes(no).fdata(end,3)
                    obj.boreholes(no).Zmax = str2double( obj.handles.bottomZ.String );
                    obj.boreholes(no).fdata(end,3) = obj.boreholes(no).Zmax;
                else
                    % append new value to fdata
                    obj.boreholes(no).Zmax = str2double( obj.handles.bottomZ.String );
                    obj.boreholes(no).fdata = [obj.boreholes(no).fdata;
                        obj.boreholes(no).X obj.boreholes(no).Y obj.boreholes(no).Z];
                end
                obj.notify('coordinatesChanged');
            end
        end
        function Zsurf(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                obj.boreholes(no).Z_surf = str2double( obj.handles.Zsurf.String );
                obj.notify('boreholeEdited');
            end
        end
        function Zwater(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                obj.boreholes(no).Z_water = str2double( obj.handles.Zwater.String );
                obj.notify('boreholeEdited');
            end
        end
        function diameter(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                obj.boreholes(no).diam = str2double( obj.handles.diameter.String );
                obj.notify('boreholeEdited');
            end
        end
        function contS(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                [file, rep] = uigetfile('*.con','Constraints file - Velocity');
                if file==0
                    return
                end
                cont = load([rep,file]);
                [scont.x,scont.y,scont.z,~] = Borehole.project(obj.boreholes(no).fdata,...
                    cont(:,1),obj.boreholes(no).name);
                scont.x = scont.x(:);
                scont.y = scont.y(:);
                scont.z = scont.z(:);
                scont.valeur = 1./cont(:,2);  % we want slowness
                if size(cont,2)==3
                    % http://math.stackexchange.com/questions/269216/inverse-of-random-variable
                    scont.variance = cont(:,3)./cont(:,2).^4;
                else
                    scont.variance = zeros(size(cont(:,2)));
                end
                obj.boreholes(no).scont = scont;
                obj.notify('boreholeEdited');
            end
        end
        function contA(obj, varargin)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                [file, rep] = uigetfile('*.con','Constraints file - Attenuation');
                if file==0
                    return
                end
                cont = load([rep,file]);
                [acont.x,acont.y,acont.z,~] = Borehole.project(obj.boreholes(no).fdata,...
                    cont(:,1),obj.boreholes(no).name);
                acont.valeur = cont(:,2);
                if size(cont,2)==3
                    acont.variance = cont(:,3);
                else
                    acont.variance = zeros(size(cont(:,2)));
                end
                obj.boreholes(no).acont = acont;
                obj.notify('boreholeEdited');
            end
        end

        function updateList(obj,varargin)
            value = [];
            if nargin==2
                value = varargin{1};
            end
            if ~isempty(obj.boreholes)
                names = cell(1,length(obj.boreholes));
                for n=1:length(obj.boreholes)
                    names{n} = obj.boreholes(n).name;
                end
                obj.handles.listBH.String = names;
                obj.handles.listBH.Value = length(obj.boreholes);
            else
                obj.handles.listBH.String = '';
                obj.handles.listBH.Value = 1;
            end
            if ~isempty(value)
                obj.handles.listBH.Value = value;
            end
        end
        function updateEdits(obj)
            no = obj.handles.listBH.Value;
            if no>0 && no<=length(obj.boreholes)
                obj.handles.topX.String = num2str( obj.boreholes(no).X );
                obj.handles.topY.String = num2str( obj.boreholes(no).Y );
                obj.handles.topZ.String = num2str( obj.boreholes(no).Z );
                obj.handles.bottomX.String = num2str( obj.boreholes(no).Xmax );
                obj.handles.bottomY.String = num2str( obj.boreholes(no).Ymax );
                obj.handles.bottomZ.String = num2str( obj.boreholes(no).Zmax );
                obj.handles.Zsurf.String = num2str( obj.boreholes(no).Z_surf );
                obj.handles.Zwater.String = num2str( obj.boreholes(no).Z_water );
                obj.handles.diameter.String = num2str( obj.boreholes(no).diam );
            else
                obj.handles.topX.String = '';
                obj.handles.topY.String = '';
                obj.handles.topZ.String = '';
                obj.handles.bottomX.String = '';
                obj.handles.bottomY.String = '';
                obj.handles.bottomZ.String = '';
                obj.handles.Zsurf.String = '';
                obj.handles.Zwater.String = '';
                obj.handles.diameter.String = '';
            end
        end
    end
end
